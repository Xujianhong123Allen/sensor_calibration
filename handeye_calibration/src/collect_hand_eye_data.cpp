#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <sensor_msgs/Image.h>
#include <geometry_msgs/PoseStamped.h>
#include <cv_bridge/cv_bridge.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <iostream>
#include <vector>

#include <tf/transform_listener.h>

class Collect_Hand_Eye_Data
{
public:
    Collect_Hand_Eye_Data(ros::NodeHandle &nh);
    ~Collect_Hand_Eye_Data();

    void read_params();
    void image_pose_cb(const sensor_msgs::Image::ConstPtr &image_msg, const geometry_msgs::PoseStamped::ConstPtr &pose_msg);

private:
    ros::NodeHandle nh_;

    std::string setting_file;
    std::string data_file_path;
    std::string image_topic;
    std::string marker_pose_topic;

    cv::FileStorage marker_poses_yaml;
    cv::FileStorage image_list_yaml;

    int IMAGE_COUNT = 0;

    std::vector<cv::Mat> vImages;
    std::vector<cv::Mat> vRwm;
    std::vector<cv::Mat> vtwm;
};

Collect_Hand_Eye_Data::Collect_Hand_Eye_Data(ros::NodeHandle &nh)
{
    nh_ = nh;
    // 1. 加载标定文件的参数
    if (nh.getParam("cfg", setting_file))
    {
        std::cout << "参数设置文件:" << setting_file << std::endl;
    }
    else
    {
        std::cerr << "标定文件目录不存在" << std::endl;
        return;
    }
    if (nh.getParam("data", data_file_path)) // 数据保存的目录
    {
        std::cout << "数据存放目录:" << data_file_path << std::endl;
    }
    else
    {
        std::cerr << "数据文件目录不存在" << std::endl;
        return;
    }

    read_params();

    time_t nowtime = time(NULL);
    struct tm *p;
    p = gmtime(&nowtime);
    std::string str_time;
    str_time = std::to_string(1900 + p->tm_year) + "-" + std::to_string(1 + p->tm_mon) + "-" + std::to_string(p->tm_mday) + "-" + std::to_string(8 + p->tm_hour) + "-" + std::to_string(p->tm_min);
    data_file_path = data_file_path + str_time + "/";
    if (0 == mkdir(data_file_path.c_str(), 0755))
    {
        std::cout << data_file_path << "创建成功！" << std::endl;
    }
    else
    {
        std::cout << "文件夹创建失败！" << std::endl;
        return;
    }

    // 创建 marker_poses.yaml
    std::string strMarkerPosesFile = data_file_path + "marker_pose.yaml";
    marker_poses_yaml = cv::FileStorage(strMarkerPosesFile, cv::FileStorage::WRITE | cv::FileStorage::APPEND | cv::FileStorage::FORMAT_YAML);
    if (!marker_poses_yaml.isOpened())
    {
        std::cout << "无法打开[ " << strMarkerPosesFile.c_str() << " ]" << std::endl;
        return;
    }

    //创建 image_list.yaml
    std::string strImageListFile = data_file_path + "image_list.yaml";
    image_list_yaml = cv::FileStorage(strImageListFile, cv::FileStorage::WRITE | cv::FileStorage::APPEND | cv::FileStorage::FORMAT_YAML);
    if (!image_list_yaml.isOpened())
    {
        std::cout << "无法打开[ " << strImageListFile.c_str() << " ]" << std::endl;
        return;
    }

    marker_poses_yaml << "Twm"
                      << "[";
    image_list_yaml << "images"
                    << "[";

    message_filters::Subscriber<sensor_msgs::Image> image_sub_(nh, image_topic, 1);
    message_filters::Subscriber<geometry_msgs::PoseStamped> pose_sub_(nh, marker_pose_topic, 1);
    typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, geometry_msgs::PoseStamped> MySyncPolicy;
    message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), image_sub_, pose_sub_);
    sync.registerCallback(boost::bind(&Collect_Hand_Eye_Data::image_pose_cb, this, _1, _2));

    ros::spin();
}

Collect_Hand_Eye_Data::~Collect_Hand_Eye_Data()
{
}

void Collect_Hand_Eye_Data::read_params()
{
    cv::FileStorage fs(setting_file, cv::FileStorage::READ | cv::FileStorage::FORMAT_YAML);
    if (!fs.isOpened())
    {
        std::cout << " [ " + setting_file + " ] 文件打开失败" << std::endl;
        return;
    }

    cv::Mat lidar_camera_T;
    image_topic = std::string(fs["Image_Topic"]);
    marker_pose_topic = std::string(fs["Marker_Pose_Topic"]);

    fs.release();
}

// function: 不阻塞的按键检测
bool kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return true;
    }
    return false;
}

void Collect_Hand_Eye_Data::image_pose_cb(const sensor_msgs::Image::ConstPtr &image_msg, const geometry_msgs::PoseStamped::ConstPtr &pose_msg)
{
    cv_bridge::CvImageConstPtr cv_ptr;
    try
    {
        cv_ptr = cv_bridge::toCvShare(image_msg);
    }
    catch (cv_bridge::Exception &e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    cv::imshow("Image", cv_ptr->image);
    cv::waitKey(1);

    /******************************************************************************************************/
    // UR3 末端执行器和camera之间的手眼标定
    // tf::TransformListener listener;
    // tf::StampedTransform transform;

    // try
    // {
    //     listener.waitForTransform("/base", "/tool0_controller", ros::Time(0), ros::Duration(0.2));
    //     listener.lookupTransform("/base", "/tool0_controller", ros::Time(0), transform);
    // }
    // catch (tf::TransformException &ex)
    // {
    //     ROS_ERROR("%s", ex.what());
    //     return;
    // }
    // tf::Transform transform_tool_end_effector;
    // transform_tool_end_effector.setOrigin(tf::Vector3(0.0, 0, 0.19836));
    // tf::Quaternion q;
    // q.setRPY(0, 0, -3.141592653 / 4.0 * 3);
    // transform_tool_end_effector.setRotation(q);
    // tf::Transform transform_base_tool;
    // transform_base_tool.setOrigin(tf::Vector3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z()));
    // transform_base_tool.setRotation(tf::Quaternion(transform.getRotation().x(), transform.getRotation().y(), transform.getRotation().z(), transform.getRotation().w()));
    // tf::Transform transform_base_end_effector;
    // transform_base_end_effector = transform_base_tool * transform_tool_end_effector;
    /******************************************************************************************************/

    if (kbhit())
    {
        int key = getchar();
        if (key == 's' | key == 'S')
        {
            std::string image_name = data_file_path + std::to_string(IMAGE_COUNT) + ".png";
            cv::imwrite(image_name, cv_ptr->image);
            image_list_yaml << image_name.c_str();

            //保存maker位姿, Twm
            float x = pose_msg->pose.orientation.x;
            float y = pose_msg->pose.orientation.y;
            float z = pose_msg->pose.orientation.z;
            float w = pose_msg->pose.orientation.w;

            /******************************************************************************************************/
            // UR3 末端执行器和camera之间的手眼标定
            // float x = transform_base_end_effector.getRotation().x();
            // float y = transform_base_end_effector.getRotation().y();
            // float z = transform_base_end_effector.getRotation().z();
            // float w = transform_base_end_effector.getRotation().w();
            /******************************************************************************************************/

            float xx = x * x;
            float yy = y * y;
            float zz = z * z;
            float xy = x * y;
            float wz = w * z;
            float wy = w * y;
            float xz = x * z;
            float yz = y * z;
            float wx = w * x;
            cv::Mat Rwm = (cv::Mat_<float>(3, 3) << 1.0f - 2 * (yy + zz), 2 * (xy - wz), 2 * (wy + xz),
                           2 * (xy + wz), 1.0f - 2 * (xx + zz), 2 * (yz - wx),
                           2 * (xz - wy), 2 * (yz + wx), 1.0f - 2 * (xx + yy));

            cv::Mat twm = (cv::Mat_<float>(3, 1) << pose_msg->pose.position.x * 1000, pose_msg->pose.position.y * 1000, pose_msg->pose.position.z * 1000);

            /******************************************************************************************************/
            // UR3 末端执行器和camera之间的手眼标定
            // cv::Mat twm = (cv::Mat_<float>(3, 1) << transform_base_end_effector.getOrigin().x() * 1000, transform_base_end_effector.getOrigin().y() * 1000, transform_base_end_effector.getOrigin().z() * 1000);
            /******************************************************************************************************/

            std::string MP_tag = "marker_poses" + std::to_string(IMAGE_COUNT);

            marker_poses_yaml << "[";
            marker_poses_yaml << Rwm;
            marker_poses_yaml << twm;
            marker_poses_yaml << "]";

            IMAGE_COUNT++;
            ROS_INFO("保存图像%d张，保存Rwm, twm完毕", IMAGE_COUNT);
        }
        if (key == 'e' | key == 'E')
        {
            marker_poses_yaml << "]";
            marker_poses_yaml << "Num" << IMAGE_COUNT;
            marker_poses_yaml.release();

            image_list_yaml << "]";
            image_list_yaml << "Num" << IMAGE_COUNT;
            image_list_yaml.release();

            std::cout << "图片、ｍarker位姿搜集完成，总共收集: " << IMAGE_COUNT << "张，保存于：" << std::endl
                      << data_file_path << std::endl;

            exit(0);
        }
    }
}

int main(int argc, char **argv)
{

    // ROS初始化及话题订阅

    ros::init(argc, argv, "Collect_Hand_Eye_Data");
    ros::NodeHandle nh("~");

    Collect_Hand_Eye_Data *collect_hand_eye_data = new Collect_Hand_Eye_Data(nh);

    return 0;
}