#include <cam_lidar_calibration/projector.h>

Projection::Projection(ros::NodeHandle &nh)
{
    // 1. 加载标定文件的参数
    if (nh.getParam("cfg", calibration_file))
    {
        std::cout << "标定文件 :" << calibration_file << std::endl;
    }
    else
    {
        std::cerr << "标定文件目录不存在" << std::endl
                  << "命令: rosrun calibration collect_hand_eye_data cfg:=/home/jh/birl/module_robot_ws/src/sensor_calibration" << std::endl;
        return;
    }

    read_extrinsic();

    // 订阅图像和点云数据并进行时间戳近似同步
    pointcloud_pub_ = nh.advertise<sensor_msgs::PointCloud2>("/Syn/velodyne_points", 1);
    image_pub_ = nh.advertise<sensor_msgs::Image>("/Syn/image", 1);

    message_filters::Subscriber<sensor_msgs::Image> image_sub(nh, "/camera/color/image_raw", 1);
    message_filters::Subscriber<sensor_msgs::PointCloud2> velodyne_sub(nh, "/velodyne_points", 1);
    typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::PointCloud2> MySyncPolicy;
    message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), image_sub, velodyne_sub);
    sync.registerCallback(boost::bind(&Projection::image_pointcolud_cb, this, _1, _2));

    reproject_image_pub_ = nh.advertise<sensor_msgs::Image>("/reprojected_image", 1, true);

    ros::spin();
}

Projection::~Projection()
{
}

void Projection::read_extrinsic() //读取标定文件
{
    cv::FileStorage fs(calibration_file, cv::FileStorage::READ | cv::FileStorage::FORMAT_YAML);
    if (!fs.isOpened())
    {
        std::cout << " [ " + calibration_file + " ] 文件打开失败" << std::endl;
        return;
    }

    cv::Mat lidar_camera_T;
    fs["lidar_camera_extrinsic"] >> lidar_camera_T;

    fs["camera_matrix"] >> camera_matrix;
    fs["camera_distortion_coefficients"] >> distortion_coefficients;
    image_width = fs["camera_image_width"];
    image_height = fs["camera_image_height"];

    std::cout << "camera K:\n" << camera_matrix << std::endl;
    std::cout << "camera D:\n" << distortion_coefficients << std::endl;
    std::cout << "LiDAR to camera T:\n" << lidar_camera_T << std::endl;

    fs.release();

    Eigen::Matrix3d R;
    R << lidar_camera_T.at<double>(0, 0), lidar_camera_T.at<double>(0, 1), lidar_camera_T.at<double>(0, 2),
        lidar_camera_T.at<double>(1, 0), lidar_camera_T.at<double>(1, 1), lidar_camera_T.at<double>(1, 2),
        lidar_camera_T.at<double>(2, 0), lidar_camera_T.at<double>(2, 1), lidar_camera_T.at<double>(2, 2);

    Eigen::Quaterniond q(R);
    q.normalize();

    Eigen::Vector3d t(lidar_camera_T.at<double>(0, 3), lidar_camera_T.at<double>(1, 3), lidar_camera_T.at<double>(2, 3));

    transform_lidar_to_camera.setOrigin(tf::Vector3(t(0), t(1), t(2)));
    transform_lidar_to_camera.setRotation(tf::Quaternion(q.x(), q.y(), q.z(), q.w()));
}

void Projection::image_pointcolud_cb(const sensor_msgs::Image::ConstPtr &ori_image, const sensor_msgs::PointCloud2::ConstPtr &ori_pointcloud)
{
    sensor_msgs::PointCloud2 syn_pointcloud = *ori_pointcloud;
    sensor_msgs::Image syn_image = *ori_image;


    pcl::PointCloud<pcl::PointXYZI> laserCloudIn;
    pcl::fromROSMsg(syn_pointcloud, laserCloudIn);

    
    
    std::vector<Eigen::Vector2d> reproject_pointclouds;

    cv_bridge::CvImagePtr cv_image_src;
    cv_image_src = cv_bridge::toCvCopy(ori_image, "bgr8");

    geometry_msgs::TransformStamped tf_msg;
    tf_msg.transform.rotation.w = transform_lidar_to_camera.inverse().getRotation().w();
    tf_msg.transform.rotation.x = transform_lidar_to_camera.inverse().getRotation().x();
    tf_msg.transform.rotation.y = transform_lidar_to_camera.inverse().getRotation().y();
    tf_msg.transform.rotation.z = transform_lidar_to_camera.inverse().getRotation().z();
    tf_msg.transform.translation.x = transform_lidar_to_camera.inverse().getOrigin().x();
    tf_msg.transform.translation.y = transform_lidar_to_camera.inverse().getOrigin().y();
    tf_msg.transform.translation.z = transform_lidar_to_camera.inverse().getOrigin().z();

    sensor_msgs::PointCloud2 cloud_reproject_tf_ros;
    tf2::doTransform(syn_pointcloud, cloud_reproject_tf_ros, tf_msg);
    pcl::PointCloud<pcl::PointXYZI> cloud_reproject_tf;
    pcl::fromROSMsg(cloud_reproject_tf_ros, cloud_reproject_tf);

    int cloudSize = cloud_reproject_tf.points.size();

    for (int i = 0; i < cloudSize; i++)
    {
        double tmpxC = cloud_reproject_tf.points[i].x / cloud_reproject_tf.points[i].z;
        double tmpyC = cloud_reproject_tf.points[i].y / cloud_reproject_tf.points[i].z;
        double tmpzC = cloud_reproject_tf.points[i].z;
        double dis = pow(cloud_reproject_tf.points[i].x * cloud_reproject_tf.points[i].x + cloud_reproject_tf.points[i].y * cloud_reproject_tf.points[i].y +
                             cloud_reproject_tf.points[i].z * cloud_reproject_tf.points[i].z,
                         0.5);
        cv::Point2d planepointsC;
        int range = std::min(round((dis / 30.0) * 49), 49.0);

        // Applying the distortion
        double r2 = tmpxC * tmpxC + tmpyC * tmpyC;
        double r1 = pow(r2, 0.5);
        double a0 = std::atan(r1);
        double a1;
        a1 = a0 * (1 + distortion_coefficients.at<double>(0, 0) * pow(a0, 2) + distortion_coefficients.at<double>(0, 1) * pow(a0, 4) +
                   distortion_coefficients.at<double>(0, 2) * pow(a0, 6) + distortion_coefficients.at<double>(0, 3) * pow(a0, 8));

        
        planepointsC.x = (a1 / r1) * tmpxC;
        planepointsC.y = (a1 / r1) * tmpyC;

        planepointsC.x = camera_matrix.at<double>(0, 0) * planepointsC.x + camera_matrix.at<double>(0, 2);
        planepointsC.y = camera_matrix.at<double>(1, 1) * planepointsC.y + camera_matrix.at<double>(1, 2);

        if (planepointsC.y >= 0 and planepointsC.y < image_height and planepointsC.x >= 0 and planepointsC.x < image_width and
            tmpzC >= 0 and std::abs(tmpxC) <= 1.35)
        {

            int point_size = 2;
            cv::circle(cv_image_src->image,
                       cv::Point(planepointsC.x, planepointsC.y), point_size,
                       CV_RGB(255 * colmap[50 - range][0], 255 * colmap[50 - range][1], 255 * colmap[50 - range][2]), -1);
        }
    }

    pointcloud_pub_.publish(syn_pointcloud);
    image_pub_.publish(syn_image);
    reproject_image_pub_.publish(cv_image_src->toImageMsg());
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "project_pointcloud_to_image");
    ros::NodeHandle nh("~");

    Projection *p = new Projection(nh);

    // ros::spin();
    ros::shutdown();
    return 0;
}
