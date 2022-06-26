#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <sensor_msgs/PointCloud2.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <visualization_msgs/MarkerArray.h>
#include <tf/transform_datatypes.h>
#include <tf/transform_broadcaster.h>
#include <tf2_ros/static_transform_broadcaster.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/filter.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/registration/ndt.h>
#include <pcl/filters/approximate_voxel_grid.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/registration/icp.h>
#include <pcl/registration/icp_nl.h>
#include <pcl/features/normal_3d_omp.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/features/fpfh_omp.h>
#include <pcl/registration/transformation_estimation_svd.h>
#include <Eigen/Core>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

// 对两个来自LOAM的点云地图进行ICP配准
class Multi_Map_Fusion
{
public:
    Multi_Map_Fusion(ros::NodeHandle &nh);
    ~Multi_Map_Fusion();

    void cloud_map_cb(const sensor_msgs::PointCloud2::ConstPtr &pointcloud_msg1, const sensor_msgs::PointCloud2::ConstPtr &pointcloud_msg2);
    bool icp_match(const pcl::PointCloud<pcl::PointXYZI>::Ptr &cloud_XYZI1, const pcl::PointCloud<pcl::PointXYZI>::Ptr &cloud_XYZI2, Eigen::Matrix4d &trans, double &fitness_score);

private:
    ros::Publisher registered_cloud_map_pub;

    ros::Publisher merge_cloud_map_pub;

    bool match_init = false;

    Eigen::Matrix4d T_G0_to_G6_init;
    Eigen::Matrix4d T_G0_to_G6_current;

    geometry_msgs::TransformStamped static_transform_stamped;
    tf2_ros::StaticTransformBroadcaster static_broadcaster;

    double init_x, init_y, init_z, init_roll, init_pitch, init_yaw;
    int map_match_min_num = 1000;
    double max_icp_fittness_score = 1;

    std::string cfg_file_path;
};

Multi_Map_Fusion::Multi_Map_Fusion(ros::NodeHandle &nh)
{
    // 1. 加载标定文件的参数
    if (nh.getParam("cfg", cfg_file_path))
    {
        std::cout << "参数配置文件 :" << cfg_file_path << std::endl;
    }
    else
    {
        std::cerr << "参数配置文件目录不存在" << std::endl;
        return;
    }

    cv::FileStorage fs(cfg_file_path, cv::FileStorage::READ | cv::FileStorage::FORMAT_YAML);
    if (!fs.isOpened())
    {
        std::cout << " [ " + cfg_file_path + " ] 文件打开失败" << std::endl;
        return;
    }

    init_x = fs["init_transform"]["x"];
    init_y = fs["init_transform"]["y"];
    init_z = fs["init_transform"]["z"];
    init_roll = fs["init_transform"]["roll"];
    init_pitch = fs["init_transform"]["pitch"];
    init_yaw = fs["init_transform"]["yaw"];

    map_match_min_num = fs["map_match_min_num"];
    std::cout << "file map_match_min_num: " << map_match_min_num << std::endl;

    max_icp_fittness_score = fs["max_icp_fittness_score"];


    merge_cloud_map_pub = nh.advertise<sensor_msgs::PointCloud2>("/merge_cloud_map", 1, true);

    // 同时订阅两个点云地图
    message_filters::Subscriber<sensor_msgs::PointCloud2> cloud_map_sub1(nh, "/G6/laser_cloud_map", 1);
    message_filters::Subscriber<sensor_msgs::PointCloud2> cloud_map_sub2(nh, "/G0/laser_cloud_map", 1); // G0是中心，其他点云地图和位姿数据都要转到G0上
    typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::PointCloud2, sensor_msgs::PointCloud2> MySyncPolicy_map;
    message_filters::Synchronizer<MySyncPolicy_map> sync_map(MySyncPolicy_map(3), cloud_map_sub1, cloud_map_sub2);
    sync_map.registerCallback(boost::bind(&Multi_Map_Fusion::cloud_map_cb, this, _1, _2));

    ros::spin();
}

Multi_Map_Fusion::~Multi_Map_Fusion()
{
}

void Multi_Map_Fusion::cloud_map_cb(const sensor_msgs::PointCloud2::ConstPtr &pointcloud_msg1, const sensor_msgs::PointCloud2::ConstPtr &pointcloud_msg2)
{
    pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_XYZI1(new pcl::PointCloud<pcl::PointXYZI>);
    pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_XYZI2(new pcl::PointCloud<pcl::PointXYZI>);

    pcl::fromROSMsg(*pointcloud_msg1, *cloud_XYZI1);
    pcl::fromROSMsg(*pointcloud_msg2, *cloud_XYZI2);

    static std::chrono::time_point<std::chrono::system_clock> start, end;

    if (match_init == false)
    {
        static bool once_time = false;
        if (once_time == false)
        {
            start = std::chrono::system_clock::now();
            once_time = true;
        }
        // std::cout << "size: " << cloud_XYZRGBA1->points.size() << " " << cloud_XYZRGBA2->points.size() << std::endl;
        if ((cloud_XYZI1->points.size() > map_match_min_num) && (cloud_XYZI2->points.size() > map_match_min_num)) // 开始校准雷达初始位姿变换矩阵
        {
            Eigen::Matrix4d tran_mat;

            tran_mat.setIdentity();
            Eigen::Translation3d init_translation(init_x, init_y, init_z);
            Eigen::AngleAxisd init_rotation_x(init_roll, Eigen::Vector3d::UnitX());
            Eigen::AngleAxisd init_rotation_y(init_pitch, Eigen::Vector3d::UnitY());
            Eigen::AngleAxisd init_rotation_z(init_yaw, Eigen::Vector3d::UnitZ()); //偏航角从上往下看逆时针为正

            tran_mat = (init_translation * init_rotation_z * init_rotation_y * init_rotation_x).matrix();
            // std::cout << "tran_mat:\n" << tran_mat << std::endl;
            double fitness_score = 0;
            if (icp_match(cloud_XYZI1, cloud_XYZI2, tran_mat, fitness_score) == true)
            {
                std::cout << "Fitness score: " << fitness_score << std::endl;
                if (fitness_score > 1)
                {
                    return;
                }
            }
            else
            {
                return;
            }
            T_G0_to_G6_init = tran_mat;

            std::cout << "雷达初始坐标系变换矩阵估计完成, 变换矩阵T base->distribution:\n "
                      << T_G0_to_G6_init << std::endl;

            end = std::chrono::system_clock::now();
            std::chrono::duration<float> elapsed_seconds = end - start;
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "计估计时间： " << double(duration.count()) / 1000000 << "s \n"
                      << std::endl;

            match_init = true;

            // 发布静态坐标world->laser_initG6
            // 父节点
            static_transform_stamped.header.frame_id = "laser_initG0";

            // 子节点
            static_transform_stamped.child_frame_id = "laser_initG6";

            Eigen::Matrix3d rotation_matrix;
            rotation_matrix << T_G0_to_G6_init(0, 0), T_G0_to_G6_init(0, 1), T_G0_to_G6_init(0, 2),
                T_G0_to_G6_init(1, 0), T_G0_to_G6_init(1, 1), T_G0_to_G6_init(1, 2),
                T_G0_to_G6_init(2, 0), T_G0_to_G6_init(2, 1), T_G0_to_G6_init(2, 2);
            Eigen::Quaterniond quat(rotation_matrix);

            static_transform_stamped.transform.translation.x = T_G0_to_G6_init(0, 3);
            static_transform_stamped.transform.translation.y = T_G0_to_G6_init(1, 3);
            static_transform_stamped.transform.translation.z = T_G0_to_G6_init(2, 3);
            static_transform_stamped.transform.rotation.x = quat.x();
            static_transform_stamped.transform.rotation.y = quat.y();
            static_transform_stamped.transform.rotation.z = quat.z();
            static_transform_stamped.transform.rotation.w = quat.w();

            // tf2 广播对象发布静态坐标变换
            static_broadcaster.sendTransform(static_transform_stamped);
        }
    }
    else
    {
        pcl::PointCloud<pcl::PointXYZI>::Ptr registered_cloud_map(new pcl::PointCloud<pcl::PointXYZI>);
        pcl::transformPointCloud(*cloud_XYZI1, *registered_cloud_map, T_G0_to_G6_init.cast<float>()); // 使用地图中的匹配

        // 对两个点云地图进行融合
        pcl::PointCloud<pcl::PointXYZI>::Ptr fusion_cloud_map(new pcl::PointCloud<pcl::PointXYZI>);
        copyPointCloud(*cloud_XYZI2, *fusion_cloud_map);
        *fusion_cloud_map += *registered_cloud_map;

        // 最后对该点云点图进行降采样
        pcl::PointCloud<pcl::PointXYZI> fusion_cloud_map_filtered;
        pcl::VoxelGrid<pcl::PointXYZI> downSizeFilter;
        downSizeFilter.setInputCloud(fusion_cloud_map);
        downSizeFilter.setLeafSize(0.3, 0.3, 0.3);
        downSizeFilter.filter(fusion_cloud_map_filtered);

        sensor_msgs::PointCloud2 fusion_map_msg;
        pcl::toROSMsg(fusion_cloud_map_filtered, fusion_map_msg);
        fusion_map_msg.header.frame_id = "laser_initG0";
        merge_cloud_map_pub.publish(fusion_map_msg);
    }
}

bool Multi_Map_Fusion::icp_match(const pcl::PointCloud<pcl::PointXYZI>::Ptr &cloud_XYZI1, const pcl::PointCloud<pcl::PointXYZI>::Ptr &cloud_XYZI2, Eigen::Matrix4d &trans, double &fitness_score)
{
    pcl::PointCloud<pcl::PointXYZINormal>::Ptr cloud_in1(new pcl::PointCloud<pcl::PointXYZINormal>);
    pcl::PointCloud<pcl::PointXYZINormal>::Ptr cloud_in2(new pcl::PointCloud<pcl::PointXYZINormal>);

    copyPointCloud(*cloud_XYZI1, *cloud_in1);
    copyPointCloud(*cloud_XYZI2, *cloud_in2);

    std::vector<int> indices;
    pcl::removeNaNFromPointCloud<pcl::PointXYZINormal>(*cloud_in1, *cloud_in1, indices);
    pcl::removeNaNFromPointCloud<pcl::PointXYZINormal>(*cloud_in2, *cloud_in2, indices);

    pcl::VoxelGrid<pcl::PointXYZINormal> filter;
    filter.setLeafSize(0.1f, 0.1f, 0.1f);
    filter.setInputCloud(cloud_in1);
    filter.filter(*cloud_in1);
    filter.setInputCloud(cloud_in2);
    filter.filter(*cloud_in2);

    pcl::StatisticalOutlierRemoval<pcl::PointXYZINormal> sor;

    sor.setInputCloud(cloud_in1);
    sor.setMeanK(10);
    sor.setStddevMulThresh(1.0);
    sor.filter(*cloud_in1);

    sor.setInputCloud(cloud_in2);
    sor.filter(*cloud_in2);

    pcl::NormalEstimationOMP<pcl::PointXYZINormal, pcl::PointXYZINormal> norm_est;
    pcl::search::KdTree<pcl::PointXYZINormal>::Ptr kdtree(new pcl::search::KdTree<pcl::PointXYZINormal>());

    norm_est.setNumberOfThreads(8);
    norm_est.setSearchMethod(kdtree);
    norm_est.setKSearch(20);

    norm_est.setInputCloud(cloud_in1);
    norm_est.compute(*cloud_in1);
    norm_est.setInputCloud(cloud_in2);
    norm_est.compute(*cloud_in2);

    pcl::IterativeClosestPointWithNormals<pcl::PointXYZINormal, pcl::PointXYZINormal, float> icp;
    icp.setInputSource(cloud_in1);
    icp.setInputTarget(cloud_in2);

    icp.setMaxCorrespondenceDistance(20.0);
    icp.setMaximumIterations(300);
    icp.setTransformationEpsilon(1e-4);
    icp.setEuclideanFitnessEpsilon(1e-3);

    pcl::PointCloud<pcl::PointXYZINormal>::Ptr registered_cloud(new pcl::PointCloud<pcl::PointXYZINormal>);
    Eigen::Matrix4f init_pose = trans.cast<float>();

    icp.align(*registered_cloud, init_pose);

    // std::cout << "align icp final" << std::endl;

    bool icp_success;

    if (icp.hasConverged() && (icp.getFitnessScore() < max_icp_fittness_score))
    {
        trans = icp.getFinalTransformation().cast<double>();
        icp_success = true;
        fitness_score = icp.getFitnessScore();
    }
    else
    {
        std::cout << "icp.getFitnessScore: " << icp.getFitnessScore() << std::endl;
        icp_success = false;
        std::cout << "ICP align failed" << std::endl;
    }

    return icp_success;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "multi_map_fusion");
    ros::NodeHandle nh("~");

    Multi_Map_Fusion m(nh);

    ros::shutdown();

    return (0);
}