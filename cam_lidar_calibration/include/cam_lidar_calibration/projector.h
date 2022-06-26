//
// Created by rdcas on 2020/4/20.
//

#ifndef CAM_LIDAR_CALIBRATION_PROJECTOR_H
#define CAM_LIDAR_CALIBRATION_PROJECTOR_H

#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include <termios.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/PointCloud2.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include "pcl_ros/transforms.h"
#include "point_xyzir.h"
#include <Eigen/Dense>
#include <ros/package.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/kdtree/flann.h>
#include <pcl/common/transforms.h>
//#include <opencv/cv.hpp>
#include <opencv2/opencv.hpp>

#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <tf2_sensor_msgs/tf2_sensor_msgs.h>

class Projection
{
public:
    Projection(ros::NodeHandle &nh);
    ~Projection();

    void read_extrinsic();          //读取标定文件
    void image_pointcolud_cb(const sensor_msgs::Image::ConstPtr &ori_image, const sensor_msgs::PointCloud2::ConstPtr &ori_pointcloud);

private:
	ros::NodeHandle nh_;
	ros::Publisher image_pub_;
	ros::Publisher pointcloud_pub_;
	ros::Publisher reproject_image_pub_;

    tf::Transform transform_lidar_to_camera;

    std::string calibration_file;
    cv::Mat camera_matrix;
    cv::Mat distortion_coefficients;
    int image_width;
    int image_height;

    double colmap[50][3] = {{0, 0, 0.5385}, {0, 0, 0.6154}, {0, 0, 0.6923}, {0, 0, 0.7692}, {0, 0, 0.8462}, {0, 0, 0.9231}, {0, 0, 1.0000}, {0, 0.0769, 1.0000}, 
                            {0, 0.1538, 1.0000}, {0, 0.2308, 1.0000}, {0, 0.3846, 1.0000}, {0, 0.4615, 1.0000}, {0, 0.5385, 1.0000}, {0, 0.6154, 1.0000}, {0, 0.6923, 1.0000}, 
                            {0, 0.7692, 1.0000}, {0, 0.8462, 1.0000}, {0, 0.9231, 1.0000}, {0, 1.0000, 1.0000}, {0.0769, 1.0000, 0.9231}, {0.1538, 1.0000, 0.8462}, 
                            {0.2308, 1.0000, 0.7692}, {0.3077, 1.0000, 0.6923}, {0.3846, 1.0000, 0.6154}, {0.4615, 1.0000, 0.5385}, {0.5385, 1.0000, 0.4615}, 
                            {0.6154, 1.0000, 0.3846}, {0.6923, 1.0000, 0.3077}, {0.7692, 1.0000, 0.2308}, {0.8462, 1.0000, 0.1538}, {0.9231, 1.0000, 0.0769}, 
                            {1.0000, 1.0000, 0}, {1.0000, 0.9231, 0}, {1.0000, 0.8462, 0}, {1.0000, 0.7692, 0}, {1.0000, 0.6923, 0}, {1.0000, 0.6154, 0}, {1.0000, 0.5385, 0}, 
                            {1.0000, 0.4615, 0}, {1.0000, 0.3846, 0}, {1.0000, 0.3077, 0}, {1.0000, 0.2308, 0}, {1.0000, 0.1538, 0}, {1.0000, 0.0769, 0}, {1.0000, 0, 0}, 
                            {0.9231, 0, 0}, {0.8462, 0, 0}, {0.7692, 0, 0}, {0.6923, 0, 0}, {0.6154, 0, 0}};

};

#endif // CAM_LIDAR_CALIBRATION_PROJECTOR_H
