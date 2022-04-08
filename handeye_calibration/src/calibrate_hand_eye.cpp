#include <ros/ros.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <time.h>
#include <sys/stat.h>


class Calibrate_Hand_Eye
{
public:
    Calibrate_Hand_Eye(ros::NodeHandle &nh);
    ~Calibrate_Hand_Eye();

    void read_params();          // 加载参数文件(标定板)、图像、marker的pose序列
    void estimate_camera_pose(); //估计相机到棋盘格的位姿
    void calibrate_handeye();    // 实现手眼标定

private:
    ros::NodeHandle nh_;

    std::string setting_file;
    std::string data_file_path;

    cv::Mat camera_matrix;
    cv::Mat distortion_coefficients;
    cv::Size boardSize;
    float squareSize;

    std::vector<cv::Mat> image_vector;
    std::vector<cv::Mat> world_marker_R_vector;
    std::vector<cv::Mat> world_marker_t_vector;
    std::vector<cv::Mat> camera_board_R_vector;
    std::vector<cv::Mat> camera_board_t_vector;
};

Calibrate_Hand_Eye::Calibrate_Hand_Eye(ros::NodeHandle &nh)
{
    // 1. 加载标定文件的参数
    if (nh.getParam("cfg", setting_file))
    {
        std::cout << "参数设置文件:" << setting_file << std::endl;
    }
    else
    {
        std::cerr << "标定文件目录不存在" << std::endl
                  << "命令: roslaunch handeye_calibration calibration_handeye.launch" << std::endl;
        return;
    }
    if (nh.getParam("data", data_file_path)) // 数据保存的目录
    {
        std::cout << "数据存放目录:" << data_file_path << std::endl;
    }
    else
    {
        std::cerr << "数据文件目录不存在" << std::endl
                  << "命令: roslaunch handeye_calibration calibration_handeye.launch" << std::endl;
        return;
    }

    
}

Calibrate_Hand_Eye::~Calibrate_Hand_Eye()
{
}

void Calibrate_Hand_Eye::read_params() // 加载参数文件(标定板)、图像、marker的pose序列
{
    // 1. 参数文件
    cv::FileStorage fs1(setting_file, cv::FileStorage::READ | cv::FileStorage::FORMAT_YAML);
    if (!fs1.isOpened())
    {
        std::cout << " [ " + setting_file + " ] 文件打开失败" << std::endl;
        return;
    }

    fs1["camera_matrix"] >> camera_matrix;
    fs1["distortion_coefficients"] >> distortion_coefficients;

    int board_width = fs1["board_width"];
    int board_height = fs1["board_height"];
    boardSize = cv::Size(board_width, board_height);

    squareSize = fs1["square_size"];
    std::cout << "棋盘格宽数： " << boardSize.width << "， 高数： " << boardSize.height << std::endl;
    std::cout << "棋盘格尺寸: " << squareSize << " mm" << std::endl;
    std::cout << "内参矩阵： \n"
              << camera_matrix << std::endl;
    std::cout << "畸变系数： \n"
              << distortion_coefficients << std::endl;
    fs1.release();

    // 2. 图像
    image_vector.clear();
    std::cout << "data_file_path: " << data_file_path << std::endl;
    std::string str_image_list_yaml = data_file_path + "image_list.yaml";
    std::cout << "str_image_list_yaml: " << str_image_list_yaml << std::endl;
    cv::FileStorage fs2(str_image_list_yaml, cv::FileStorage::READ | cv::FileStorage::FORMAT_XML);
    if (!fs2.isOpened())
    {
        std::cout << " [ " + str_image_list_yaml + " ] 文件打开失败" << std::endl;
        return;
    }
    int num = fs2["Num"];
    for (int i = 0; i < num; i++)
    {
        std::string imageFile;
        fs2["images"][i] >> imageFile;
        // cout << imageFile << endl;
        cv::Mat imageMat = cv::imread(imageFile);
        if (imageMat.empty())
        {
            std::cout << "[ " + imageFile + " ] 图像读取失败" << std::endl;
            return;
        }
        image_vector.push_back(imageMat);
    }
    fs2.release();

    // 3. marker pose
    world_marker_R_vector.clear();
    world_marker_t_vector.clear();
    std::string str_marker_pose_file = data_file_path + "marker_pose.yaml";
    std::cout << "Marker pose file: " << str_marker_pose_file << std::endl;
    cv::FileStorage fs3(str_marker_pose_file, cv::FileStorage::READ | cv::FileStorage::FORMAT_XML);
    if (!fs3.isOpened())
    {
        std::cout << "[ " + str_marker_pose_file + " ] 文件打开失败！" << std::endl;
        return;
    }
    num = fs3["Num"];
    for (int i = 0; i < num; i++)
    {
        cv::Mat Rwm, twm;
        fs3["Twm"][i][0] >> Rwm;
        fs3["Twm"][i][1] >> twm;

        world_marker_R_vector.push_back(Rwm);
        world_marker_t_vector.push_back(twm);
    }
    fs3.release();
}

void Calibrate_Hand_Eye::estimate_camera_pose()
{
    camera_board_R_vector.clear();
    camera_board_t_vector.clear();

    for (size_t i = 0; i < image_vector.size(); i++)
    {
        cv::Mat img = image_vector[i];
        std::vector<cv::Point2f> points2d_vector;
        std::vector<cv::Point3f> points3d_vector;

        // 获取角点图像坐标
        if (0 == findChessboardCorners(img, boardSize, points2d_vector))
        {
            std::cout << "图像 " << i << "提取不到角点" << std::endl;
            return;
        }
        else
        {
            cv::Mat imgGray;
            cv::cvtColor(img, imgGray, cv::COLOR_RGB2GRAY);
            //亚像素精细化
            cv::cornerSubPix(imgGray, points2d_vector, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));
            //绘制图像
        }

        // 获取角点三维坐标
        for (int i = 0; i < boardSize.height; i++)
        {
            for (int j = 0; j < boardSize.width; j++)
            {
                cv::Point3d tmpPoint;
                tmpPoint.x = j * squareSize;
                tmpPoint.y = i * squareSize;
                tmpPoint.z = 0;
                points3d_vector.push_back(tmpPoint);
            }
        }

        cv::Mat rvec, tvec, rMat;

        cv::solvePnP(points3d_vector, points2d_vector, camera_matrix, distortion_coefficients, rvec, tvec, false);
        cv::Rodrigues(rvec, rMat);

        std::cout << "2!!!!!!!!!:" << rMat << std::endl;
        std::cout << tvec << std::endl;

        camera_board_R_vector.push_back(rMat);
        camera_board_t_vector.push_back(tvec);
    }
}

void Calibrate_Hand_Eye::calibrate_handeye() // 实现手眼标定
{
    // 开始求解AX=XB方程

    cv::HandEyeCalibrationMethod method;

    // CALIB_HAND_EYE_TSAI         = 0, //!< A New Technique for Fully Autonomous and Efficient 3D Robotics Hand/Eye Calibration @cite Tsai89
    // CALIB_HAND_EYE_PARK         = 1, //!< Robot Sensor Calibration: Solving AX = XB on the Euclidean Group @cite Park94
    // CALIB_HAND_EYE_HORAUD       = 2, //!< Hand-eye Calibration @cite Horaud95
    // CALIB_HAND_EYE_ANDREFF      = 3, //!< On-line Hand-Eye Calibration @cite Andreff99
    // CALIB_HAND_EYE_DANIILIDIS   = 4
    std::string str_handeye_result_file = data_file_path + "handeye_result.yaml";
    cv::FileStorage handeye_result = cv::FileStorage(str_handeye_result_file, cv::FileStorage::WRITE | cv::FileStorage::FORMAT_YAML | cv::FileStorage::APPEND);

    for (int method_idx = 0; method_idx < 5; method_idx++)
    {
        switch (method_idx)
        {
        case 0:
            method = cv::CALIB_HAND_EYE_TSAI;
            handeye_result << "method"
                           << "CALIB_HAND_EYE_TSAI";
            std::cout << "CALIB_HAND_EYE_TSAI---------------------" << std::endl;
            break;
        case 1:
            method = cv::CALIB_HAND_EYE_PARK;
            handeye_result << "method"
                           << "ALIB_HAND_EYE_PARK";
            std::cout << "ALIB_HAND_EYE_PARK----------------------" << std::endl;
            break;
        case 2:
            method = cv::CALIB_HAND_EYE_HORAUD;
            handeye_result << "method"
                           << "CALIB_HAND_EYE_HORAUD";
            std::cout << "CALIB_HAND_EYE_HORAUD-------------------" << std::endl;
            break;
        case 3:
            method = cv::CALIB_HAND_EYE_ANDREFF;
            handeye_result << "method"
                           << "CALIB_HAND_EYE_ANDREFF";
            std::cout << "CALIB_HAND_EYE_ANDREFF------------------" << std::endl;
            break;
        case 4:
            method = cv::CALIB_HAND_EYE_DANIILIDIS;
            handeye_result << "method"
                           << "CALIB_HAND_EYE_DANIILIDIS";
            std::cout << "CALIB_HAND_EYE_DANIILIDIS--------------" << std::endl;
            break;

        default:
            method = cv::CALIB_HAND_EYE_TSAI;
            handeye_result << "method"
                           << "CALIB_HAND_EYE_TSAI";
            std::cout << "CALIB_HAND_EYE_TSAI----------------------" << std::endl;
            break;
        }

        cv::Mat R_mc, t_mc;
        cv::calibrateHandEye(world_marker_R_vector, world_marker_t_vector, camera_board_R_vector, camera_board_t_vector, R_mc, t_mc, method);

        cv::Mat Tmc = (cv::Mat_<double>(4, 4) << R_mc.at<double>(0, 0), R_mc.at<double>(0, 1), R_mc.at<double>(0, 2), t_mc.at<double>(0),
                       R_mc.at<double>(1, 0), R_mc.at<double>(1, 1), R_mc.at<double>(1, 2), t_mc.at<double>(1),
                       R_mc.at<double>(2, 0), R_mc.at<double>(2, 1), R_mc.at<double>(2, 2), t_mc.at<double>(2),
                       0, 0, 0, 1);
        std::cout << "Tmc:\n"
                  << Tmc << std::endl;

        int num = world_marker_R_vector.size();
        std::vector<cv::Mat> vT_wb;
        for (int i = 0; i < num; i++)
        {
            cv::Mat Twm = (cv::Mat_<double>(4, 4) << world_marker_R_vector[i].at<float>(0, 0), world_marker_R_vector[i].at<float>(0, 1), world_marker_R_vector[i].at<float>(0, 2), world_marker_t_vector[i].at<float>(0),
                           world_marker_R_vector[i].at<float>(1, 0), world_marker_R_vector[i].at<float>(1, 1), world_marker_R_vector[i].at<float>(1, 2), world_marker_t_vector[i].at<float>(1),
                           world_marker_R_vector[i].at<float>(2, 0), world_marker_R_vector[i].at<float>(2, 1), world_marker_R_vector[i].at<float>(2, 2), world_marker_t_vector[i].at<float>(2),
                           0, 0, 0, 1);

            cv::Mat Tcb = (cv::Mat_<double>(4, 4) << camera_board_R_vector[i].at<double>(0, 0), camera_board_R_vector[i].at<double>(0, 1), camera_board_R_vector[i].at<double>(0, 2), camera_board_t_vector[i].at<double>(0),
                           camera_board_R_vector[i].at<double>(1, 0), camera_board_R_vector[i].at<double>(1, 1), camera_board_R_vector[i].at<double>(1, 2), camera_board_t_vector[i].at<double>(1),
                           camera_board_R_vector[i].at<double>(2, 0), camera_board_R_vector[i].at<double>(2, 1), camera_board_R_vector[i].at<double>(2, 2), camera_board_t_vector[i].at<double>(2),
                           0, 0, 0, 1);

            cv::Mat Twb_tmp, Twb;
            Twb = Twm * Tmc * Tcb;
            vT_wb.push_back(Twb);
        }

        std::vector<double> r1, r2, r3, r4, r5, r6, r7, r8, r9;
        std::vector<double> t1, t2, t3;

        for (int i = 0; i < num; i++)
        {
            r1.push_back(vT_wb[i].at<double>(0, 0));
            r2.push_back(vT_wb[i].at<double>(0, 1));
            r3.push_back(vT_wb[i].at<double>(0, 2));
            r4.push_back(vT_wb[i].at<double>(1, 0));
            r5.push_back(vT_wb[i].at<double>(1, 1));
            r6.push_back(vT_wb[i].at<double>(1, 2));
            r7.push_back(vT_wb[i].at<double>(2, 0));
            r8.push_back(vT_wb[i].at<double>(2, 1));
            r9.push_back(vT_wb[i].at<double>(2, 2));

            t1.push_back(vT_wb[i].at<double>(0, 3));
            t2.push_back(vT_wb[i].at<double>(1, 3));
            t3.push_back(vT_wb[i].at<double>(2, 3));
        }

        cv::Mat mat_r1 = cv::Mat(r1);
        cv::Mat mat_r2 = cv::Mat(r2);
        cv::Mat mat_r3 = cv::Mat(r3);
        cv::Mat mat_r4 = cv::Mat(r4);
        cv::Mat mat_r5 = cv::Mat(r5);
        cv::Mat mat_r6 = cv::Mat(r6);
        cv::Mat mat_r7 = cv::Mat(r7);
        cv::Mat mat_r8 = cv::Mat(r8);
        cv::Mat mat_r9 = cv::Mat(r9);

        cv::Mat mat_t1 = cv::Mat(t1);
        cv::Mat mat_t2 = cv::Mat(t2);
        cv::Mat mat_t3 = cv::Mat(t3);

        cv::Mat mr1, mr2, mr3, mr4, mr5, mr6, mr7, mr8, mr9, mt1, mt2, mt3;
        cv::Mat stdr1, stdr2, stdr3, stdr4, stdr5, stdr6, stdr7, stdr8, stdr9, stdt1, stdt2, stdt3;

        cv::meanStdDev(mat_r1, mr1, stdr1);
        cv::meanStdDev(mat_r2, mr2, stdr2);
        cv::meanStdDev(mat_r3, mr3, stdr3);
        cv::meanStdDev(mat_r4, mr4, stdr4);
        cv::meanStdDev(mat_r5, mr5, stdr5);
        cv::meanStdDev(mat_r6, mr6, stdr6);
        cv::meanStdDev(mat_r7, mr7, stdr7);
        cv::meanStdDev(mat_r8, mr8, stdr8);
        cv::meanStdDev(mat_r9, mr9, stdr9);
        cv::meanStdDev(mat_t1, mt1, stdt1);
        cv::meanStdDev(mat_t2, mt2, stdt2);
        cv::meanStdDev(mat_t3, mt3, stdt3);

        cv::Mat mat_dev = (cv::Mat_<double>(3, 4) << stdr1.at<double>(0), stdr2.at<double>(0), stdr3.at<double>(0), stdt1.at<double>(0),
                           stdr4.at<double>(0), stdr5.at<double>(0), stdr6.at<double>(0), stdt2.at<double>(0),
                           stdr7.at<double>(0), stdr8.at<double>(0), stdr9.at<double>(0), stdt3.at<double>(0));

        handeye_result << "Tmc" << Tmc;
        handeye_result << "Standard_Deviation" << mat_dev;

        std::cout << "standard_deviation:\n"
                  << mat_dev << std::endl;
        std::cout << "-----------------------------------------------------" << std::endl;
    }

    handeye_result.release();
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "calibrate_hand_eye");
    ros::NodeHandle nh("~");

    Calibrate_Hand_Eye calib(nh);
    calib.read_params();
    calib.estimate_camera_pose();
    calib.calibrate_handeye();

    return 0;
}