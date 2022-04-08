// Optimization node
#include <string>
#include <iostream>
#include <fstream>
#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include "std_msgs/String.h"
#include <vector>
#include <math.h>
#include <Eigen/Dense>
#include "cam_lidar_calibration/calibration_data.h"
#include "point_xyzir.h"
#include "openga.h"
#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>
#include <string.h>
#include <utility>
#include <ros/package.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/kdtree/flann.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv/cv.hpp>
#include <opencv2/opencv.hpp>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include "pcl_ros/transforms.h"
#include <tf2/convert.h>
#include <Eigen/Geometry>
#include <pcl/io/pcd_io.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <Eigen/Core>

#define PI 3.141592653589793238463

int sample = 0;
bool sensor_pair = 0;
bool output = 0;
bool output2 = 0;
static cv::Mat new_K;
cv::Mat raw_image, undist_image;
pcl::PointCloud<pcl::PointXYZIR>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZIR>);
image_transport::Publisher pub_img_dist;
cv_bridge::CvImagePtr cv_ptr(new cv_bridge::CvImage);
std::vector<double> rotm2eul(cv::Mat);
double *converto_imgpts(double x, double y, double z);
void sensor_info_callback(const sensor_msgs::Image::ConstPtr &img, const sensor_msgs::PointCloud2::ConstPtr &pc);
pcl::PointCloud<pcl::PointXYZIR> organized_pointcloud(pcl::PointCloud<pcl::PointXYZIR>::Ptr input_pointcloud);

double colmap[50][3] = {{0, 0, 0.5385}, {0, 0, 0.6154}, {0, 0, 0.6923}, {0, 0, 0.7692}, {0, 0, 0.8462}, {0, 0, 0.9231}, {0, 0, 1.0000}, {0, 0.0769, 1.0000}, {0, 0.1538, 1.0000}, {0, 0.2308, 1.0000}, {0, 0.3846, 1.0000}, {0, 0.4615, 1.0000}, {0, 0.5385, 1.0000}, {0, 0.6154, 1.0000}, {0, 0.6923, 1.0000}, {0, 0.7692, 1.0000}, {0, 0.8462, 1.0000}, {0, 0.9231, 1.0000}, {0, 1.0000, 1.0000}, {0.0769, 1.0000, 0.9231}, {0.1538, 1.0000, 0.8462}, {0.2308, 1.0000, 0.7692}, {0.3077, 1.0000, 0.6923}, {0.3846, 1.0000, 0.6154}, {0.4615, 1.0000, 0.5385}, {0.5385, 1.0000, 0.4615}, {0.6154, 1.0000, 0.3846}, {0.6923, 1.0000, 0.3077}, {0.7692, 1.0000, 0.2308}, {0.8462, 1.0000, 0.1538}, {0.9231, 1.0000, 0.0769}, {1.0000, 1.0000, 0}, {1.0000, 0.9231, 0}, {1.0000, 0.8462, 0}, {1.0000, 0.7692, 0}, {1.0000, 0.6923, 0}, {1.0000, 0.6154, 0}, {1.0000, 0.5385, 0}, {1.0000, 0.4615, 0}, {1.0000, 0.3846, 0}, {1.0000, 0.3077, 0}, {1.0000, 0.2308, 0}, {1.0000, 0.1538, 0}, {1.0000, 0.0769, 0}, {1.0000, 0, 0}, {0.9231, 0, 0}, {0.8462, 0, 0}, {0.7692, 0, 0}, {0.6923, 0, 0}};

std::string pkg_loc;

struct CameraVelodyneCalibrationData
{
	std::vector<std::vector<double>> velodynenormals;
	std::vector<std::vector<double>> velodynepoints;
	std::vector<std::vector<double>> cameranormals;
	std::vector<std::vector<double>> camerapoints;
	std::vector<std::vector<double>> velodynecorners;
	std::vector<double> pixeldata;

	cv::Mat cameranormals_mat;	 // nX3
	cv::Mat camerapoints_mat;	 // nX3
	cv::Mat velodynepoints_mat;	 // nX3
	cv::Mat velodynenormals_mat; // nX3
	cv::Mat velodynecorners_mat; // nX3
	cv::Mat pixeldata_mat;		 // 1X3

} calibrationdata;

struct initial_parameters
{
	std::string camera_topic;
	std::string lidar_topic;
	bool fisheye_model;
	int lidar_ring_count;
	std::pair<int, int> grid_size;
	int square_length;						  // in millimetres
	std::pair<int, int> board_dimension;	  // in millimetres
	std::pair<int, int> cb_translation_error; // in millimetres
	cv::Mat cameramat;
	int distcoeff_num;
	cv::Mat distcoeff;
	std::pair<int, int> image_size;
} i_params;

struct Rotation
{
	double e1; // Rotation optimization variables
	double e2;
	double e3;
	std::string to_string() const
	{
		return std::string("{") + "e1:" + std::to_string(e1) + ", e2:" + std::to_string(e2) + ", e3:" + std::to_string(e3) + "}";
	}
} eul;

struct Rot_Trans
{
	double e1; // Joint (Rotation and translation) optimization variables
	double e2;
	double e3;
	double x;
	double y;
	double z;
	std::string to_string() const
	{
		return std::string("{") + "e1:" + std::to_string(e1) + ", e2:" + std::to_string(e2) + ", e3:" + std::to_string(e3) + ", x:" + std::to_string(x) + ", y:" + std::to_string(y) + ", z:" + std::to_string(z) + "}";
	}
} eul_t, eul_it;
void image_projection(Rot_Trans rot_trans);

struct Rotationcost // equivalent to y in matlab
{
	double objective1; // This is where the results of simulation is stored but not yet finalized.
};

struct Rot_Trans_cost // equivalent to y in matlab
{
	double objective2; // This is where the results of simulation is stored but not yet finalized.
};

typedef EA::Genetic<Rotation, Rotationcost> GA_Type;
typedef EA::Genetic<Rot_Trans, Rot_Trans_cost> GA_Type2;

void init_genes2(Rot_Trans &p, const std::function<double(void)> &rnd01)
{
	std::vector<double> pi_vals;
	pi_vals.push_back(PI / 18);
	pi_vals.push_back(-PI / 18);
	int RandIndex = rand() % 2;
	p.e1 = eul_t.e1 + pi_vals.at(RandIndex) * rnd01();
	RandIndex = rand() % 2;
	p.e2 = eul_t.e2 + pi_vals.at(RandIndex) * rnd01();
	RandIndex = rand() % 2;
	p.e3 = eul_t.e3 + pi_vals.at(RandIndex) * rnd01();

	std::vector<double> trans_vals;
	trans_vals.push_back(0.05);
	trans_vals.push_back(-0.05);
	RandIndex = rand() % 2;
	p.x = eul_t.x + trans_vals.at(RandIndex) * rnd01();
	RandIndex = rand() % 2;
	p.y = eul_t.y + trans_vals.at(RandIndex) * rnd01();
	RandIndex = rand() % 2;
	p.z = eul_t.z + trans_vals.at(RandIndex) * rnd01();
}

double rotation_fitness_func(double e1, double e2, double e3)
{
	tf::Matrix3x3 rot;
	rot.setRPY(e1, e2, e3);
	cv::Mat tmp_rot = (cv::Mat_<double>(3, 3) << rot.getRow(0)[0], rot.getRow(0)[1], rot.getRow(0)[2],
					   rot.getRow(1)[0], rot.getRow(1)[1], rot.getRow(1)[2],
					   rot.getRow(2)[0], rot.getRow(2)[1], rot.getRow(2)[2]);

	cv::Mat normals = calibrationdata.cameranormals_mat * tmp_rot.t(); // camera normals in lidar frame
	cv::Mat normal_diff = normals - calibrationdata.velodynenormals_mat;
	cv::Mat normal_square = normal_diff.mul(normal_diff); // square the xyz components of normal_diff
	cv::Mat summed_norm_diff;
	cv::reduce(normal_square, summed_norm_diff, 1, CV_REDUCE_SUM, CV_64F); // add the squared terms
	cv::Mat sqrt_norm_diff;
	sqrt(summed_norm_diff, sqrt_norm_diff); // take the square root
	double sqrt_norm_sum = 0.0;
	for (int i = 0; i < sample; i++)
		sqrt_norm_sum += sqrt_norm_diff.at<double>(i); // Add the errors involved in all the vectors

	double ana = sqrt_norm_sum / sample; // divide this sum by the total number of samples

	// vectors on the board plane (w.r.t lidar frame)
	cv::Mat plane_vectors = calibrationdata.velodynepoints_mat - calibrationdata.velodynecorners_mat;
	double error_dot = 0.0;
	for (int i = 0; i < sqrt_norm_diff.rows; i++)
	{
		cv::Mat plane_vector = plane_vectors.row(i);
		plane_vector = plane_vector / norm(plane_vector);
		double temp_err_dot = pow(normals.row(i).dot(plane_vector), 2);
		error_dot += temp_err_dot;
	}
	error_dot = error_dot / sample; // dot product average

	if (output)
	{
		std::cout << "sqrt_norm_sum " << sqrt_norm_sum << std::endl;
		std::cout << "sqrt_norm_diff.rows " << sqrt_norm_diff.rows << std::endl;
		std::cout << "rotation " << tmp_rot << std::endl;
		std::cout << "normals " << normals << std::endl;
		std::cout << "normal_diff " << normal_diff << std::endl;
		std::cout << "normal_square " << normal_square << std::endl;
		std::cout << "summed_norm_diff " << summed_norm_diff << std::endl;
		std::cout << "sqrt_norm_diff " << sqrt_norm_diff << std::endl;
		std::cout << "sqrt_norm_sum " << sqrt_norm_sum << std::endl;
		std::cout << "ana " << ana << std::endl;
		std::cout << "error_dot " << error_dot << std::endl;
	}
	return error_dot + ana;
}

bool eval_solution2(const Rot_Trans &p, Rot_Trans_cost &c)
{
	const double &e1 = p.e1;
	const double &e2 = p.e2;
	const double &e3 = p.e3;
	const double &x = p.x;
	const double &y = p.y;
	const double &z = p.z;

	tf::Matrix3x3 rot;
	rot.setRPY(e1, e2, e3);
	cv::Mat tmp_rot = (cv::Mat_<double>(3, 3) << rot.getRow(0)[0], rot.getRow(0)[1], rot.getRow(0)[2],
					   rot.getRow(1)[0], rot.getRow(1)[1], rot.getRow(1)[2],
					   rot.getRow(2)[0], rot.getRow(2)[1], rot.getRow(2)[2]);

	double rot_error = rotation_fitness_func(e1, e2, e3);

	cv::Mat translation_ana = (cv::Mat_<double>(1, 3) << x, y, z);
	cv::Mat rt, t_fin, vpoints, cpoints;
	cv::Mat l_row = (cv::Mat_<double>(1, 4) << 0.0, 0.0, 0.0, 1.0);
	cv::hconcat(tmp_rot, translation_ana.t(), rt);
	cv::vconcat(rt, l_row, t_fin);
	cv::hconcat(calibrationdata.velodynepoints_mat, cv::Mat::ones(sample, 1, CV_64F), vpoints);
	cv::hconcat(calibrationdata.camerapoints_mat, cv::Mat::ones(sample, 1, CV_64F), cpoints);
	cv::Mat cp_rot = t_fin.inv() * vpoints.t();
	cp_rot = cp_rot.t();
	cv::Mat trans_diff = cp_rot - cpoints;
	trans_diff = trans_diff.mul(trans_diff);
	cv::Mat summed_norm_diff, sqrt_norm_sum, sqrt_norm_diff;
	cv::reduce(trans_diff, summed_norm_diff, 1, CV_REDUCE_SUM, CV_64F);
	sqrt(summed_norm_diff, sqrt_norm_diff);
	double summed_sqrt = 0.0;
	for (int i = 0; i < sample; i++)
	{
		summed_sqrt += sqrt_norm_diff.at<double>(i);
	}
	double error_trans = summed_sqrt / sample;

	cv::Mat meanValue, stdValue;
	cv::meanStdDev(sqrt_norm_diff, meanValue, stdValue);

	double var;
	var = stdValue.at<double>(0);

	std::vector<double> pixel_error;
	for (int i = 0; i < sample; i++)
	{
		double *my_cp = converto_imgpts(cp_rot.at<double>(i, 0),
										cp_rot.at<double>(i, 1),
										cp_rot.at<double>(i, 2));
		double *my_vp = converto_imgpts(cpoints.at<double>(i, 0),
										cpoints.at<double>(i, 1),
										cpoints.at<double>(i, 2));
		double pix_e = sqrt(pow((my_cp[0] - my_vp[0]), 2) + pow((my_cp[1] - my_vp[1]), 2)) *
					   calibrationdata.pixeldata_mat.at<double>(i);
		pixel_error.push_back(pix_e);
	}

	double error_pix = *std::max_element(pixel_error.begin(), pixel_error.end());

	c.objective2 = rot_error + var + error_trans + error_pix;

	if (output2)
	{
		std::cout << "sample " << sample << std::endl;
		std::cout << "tmp_rot " << tmp_rot << std::endl;
		std::cout << "cp_rot " << cp_rot << std::endl;
		std::cout << "t_fin " << t_fin << std::endl;
		std::cout << "translation_ana " << translation_ana << std::endl;
		std::cout << "cp_rot " << cp_rot << std::endl;
		std::cout << "calibrationdata.camerapoints_mat " << calibrationdata.camerapoints_mat << std::endl;
		std::cout << "trans_diff " << trans_diff << std::endl;
		std::cout << "summed_norm_diff " << summed_norm_diff << std::endl;
		std::cout << "sqrt_norm_diff " << sqrt_norm_diff << std::endl;
		std::cout << "summed_sqrt " << summed_sqrt << std::endl;
		std::cout << "error_trans " << error_trans << std::endl;
		std::cout << "c.objective2 " << c.objective2 << std::endl;
		std::cout << "error_pix " << error_pix << std::endl;
	}
	output2 = 0;
	return true; // solution is accepted
}

Rot_Trans mutate2(const Rot_Trans &X_base, const std::function<double(void)> &rnd01, double shrink_scale)
{
	Rot_Trans X_new;
	bool in_range;
	do
	{
		in_range = true;
		X_new = X_base;
		X_new.e1 += 0.2 * (rnd01() - rnd01()) * shrink_scale;
		in_range = in_range && (X_new.e1 >= (eul_t.e1 - PI / 18) && X_new.e1 < (eul_t.e1 + PI / 18));
		X_new.e2 += 0.2 * (rnd01() - rnd01()) * shrink_scale;
		in_range = in_range && (X_new.e2 >= (eul_t.e2 - PI / 18) && X_new.e2 < (eul_t.e2 + PI / 18));
		X_new.e3 += 0.2 * (rnd01() - rnd01()) * shrink_scale;
		in_range = in_range && (X_new.e3 >= (eul_t.e3 - PI / 18) && X_new.e3 < (eul_t.e3 + PI / 18));

		X_new.x += 0.2 * (rnd01() - rnd01()) * shrink_scale;
		in_range = in_range && (X_new.x >= (eul_t.x - 0.05) && X_new.x < (eul_t.x + 0.05));
		X_new.y += 0.2 * (rnd01() - rnd01()) * shrink_scale;
		in_range = in_range && (X_new.y >= (eul_t.y - 0.05) && X_new.y < (eul_t.y + 0.05));
		X_new.z += 0.2 * (rnd01() - rnd01()) * shrink_scale;
		in_range = in_range && (X_new.z >= (eul_t.z - 0.05) && X_new.z < (eul_t.z + 0.05));

	} while (!in_range);
	return X_new;
}

Rot_Trans crossover2(const Rot_Trans &X1,
					 const Rot_Trans &X2,
					 const std::function<double(void)> &rnd01)
{
	Rot_Trans X_new;
	double r;
	r = rnd01();
	X_new.e1 = r * X1.e1 + (1.0 - r) * X2.e1;
	r = rnd01();
	X_new.e2 = r * X1.e2 + (1.0 - r) * X2.e2;
	r = rnd01();
	X_new.e3 = r * X1.e3 + (1.0 - r) * X2.e3;
	r = rnd01();
	X_new.x = r * X1.x + (1.0 - r) * X2.x;
	r = rnd01();
	X_new.y = r * X1.y + (1.0 - r) * X2.y;
	r = rnd01();
	X_new.z = r * X1.z + (1.0 - r) * X2.z;
	return X_new;
}

double calculate_SO_total_fitness2(const GA_Type2::thisChromosomeType &X)
{
	// finalize the cost
	double final_cost = 0.0;
	final_cost += X.middle_costs.objective2;
	return final_cost;
}

// A function to show/store the results of each generation.
void SO_report_generation2(int generation_number,
						   const EA::GenerationType<Rot_Trans,
													Rot_Trans_cost> &last_generation,
						   const Rot_Trans &best_genes)
{
	eul_it.e1 = best_genes.e1;
	eul_it.e2 = best_genes.e2;
	eul_it.e3 = best_genes.e3;
	eul_it.x = best_genes.x;
	eul_it.y = best_genes.y;
	eul_it.z = best_genes.z;
}

void init_genes(Rotation &p, const std::function<double(void)> &rnd01)
{
	std::vector<double> pi_vals;
	pi_vals.push_back(PI / 8);
	pi_vals.push_back(-PI / 8);
	int RandIndex = rand() % 2;
	p.e1 = eul.e1 + pi_vals.at(RandIndex) * rnd01();
	RandIndex = rand() % 2;
	p.e2 = eul.e2 + pi_vals.at(RandIndex) * rnd01();
	RandIndex = rand() % 2;
	p.e3 = eul.e3 + pi_vals.at(RandIndex) * rnd01();
}

bool eval_solution(const Rotation &p, Rotationcost &c)
{
	const double &e1 = p.e1;
	const double &e2 = p.e2;
	const double &e3 = p.e3;

	c.objective1 = rotation_fitness_func(e1, e2, e3);

	return true; // solution is accepted
}

Rotation mutate(const Rotation &X_base,
				const std::function<double(void)> &rnd01,
				double shrink_scale)
{
	Rotation X_new;
	bool in_range;
	do
	{
		in_range = true;
		X_new = X_base;
		X_new.e1 += 0.2 * (rnd01() - rnd01()) * shrink_scale;
		in_range = in_range && (X_new.e1 >= (eul.e1 - PI / 8) && X_new.e1 < (eul.e1 + PI / 8));
		X_new.e2 += 0.2 * (rnd01() - rnd01()) * shrink_scale;
		in_range = in_range && (X_new.e2 >= (eul.e2 - PI / 8) && X_new.e2 < (eul.e2 + PI / 8));
		X_new.e3 += 0.2 * (rnd01() - rnd01()) * shrink_scale;
		in_range = in_range && (X_new.e3 >= (eul.e3 - PI / 8) && X_new.e3 < (eul.e3 + PI / 8));
	} while (!in_range);
	return X_new;
}

Rotation crossover(const Rotation &X1,
				   const Rotation &X2,
				   const std::function<double(void)> &rnd01)
{
	Rotation X_new;
	double r = rnd01();
	X_new.e1 = r * X1.e1 + (1.0 - r) * X2.e1;
	r = rnd01();
	X_new.e2 = r * X1.e2 + (1.0 - r) * X2.e2;
	r = rnd01();
	X_new.e3 = r * X1.e3 + (1.0 - r) * X2.e3;
	return X_new;
}

double calculate_SO_total_fitness(const GA_Type::thisChromosomeType &X)
{
	double final_cost = 0.0; // finalize the cost
	final_cost += X.middle_costs.objective1;
	return final_cost;
}

// A function to show/store the results of each generation.
void SO_report_generation(int generation_number,
						  const EA::GenerationType<Rotation, Rotationcost> &last_generation,
						  const Rotation &best_genes)
{
	//  std::cout
	//      <<"Generation ["<<generation_number<<"], "
	//     <<"Best ="<<last_generation.best_total_cost<<", "
	//    <<"Average ="<<last_generation.average_cost<<", "
	//   <<"Best genes =("<<best_genes.to_string()<<")"<<", "
	//  <<"Exe_time ="<<last_generation.exe_time
	//  << std::endl;
	eul_t.e1 = best_genes.e1;
	eul_t.e2 = best_genes.e2;
	eul_t.e3 = best_genes.e3;

	//  std::cout << "eul_t assign " << eul_t.e1 << " "
	//            << eul_t.e2 << " "
	//            <<  eul_t.e3 << std::endl;
}

void get_samples(const cam_lidar_calibration::calibration_data::ConstPtr &data)
{
	sample++;
	std::cout << "sample" << sample << std::endl;
	std::vector<double> camera_p;
	std::vector<double> camera_n;
	std::vector<double> velodyne_p;
	std::vector<double> velodyne_n;
	std::vector<double> velodyne_c;

	for (int i = 0; i < 3; i++)
	{
		velodyne_n.push_back(data->velodynenormal[i]);
		velodyne_p.push_back(data->velodynepoint[i] / 1000);
		camera_n.push_back(data->cameranormal[i]);
		camera_p.push_back(data->camerapoint[i] / 1000);
		velodyne_c.push_back(data->velodynecorner[i]);
	}

	calibrationdata.velodynenormals.push_back(velodyne_n);
	calibrationdata.velodynepoints.push_back(velodyne_p);
	calibrationdata.cameranormals.push_back(camera_n);
	calibrationdata.camerapoints.push_back(camera_p);
	calibrationdata.velodynecorners.push_back(velodyne_c);
	calibrationdata.pixeldata.push_back(data->pixeldata);
}

void my_flag(const std_msgs::Int8::ConstPtr &msg)
{
	// runs if you press 'o'
	if (msg->data == 2)
	{
		std::cout << "input samples collected" << std::endl;
		calibrationdata.cameranormals_mat = cv::Mat(sample, 3, CV_64F);
		calibrationdata.camerapoints_mat = cv::Mat(sample, 3, CV_64F);
		calibrationdata.velodynepoints_mat = cv::Mat(sample, 3, CV_64F);
		calibrationdata.velodynenormals_mat = cv::Mat(sample, 3, CV_64F);
		calibrationdata.velodynecorners_mat = cv::Mat(sample, 3, CV_64F);
		calibrationdata.pixeldata_mat = cv::Mat(1, sample, CV_64F);
		std::cout << "input samples collected 2" << std::endl;

		// Insert vector elements into cv::Mat for easy matrix operation
		for (int i = 0; i < sample; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				calibrationdata.camerapoints_mat.at<double>(i, j) = calibrationdata.camerapoints[i][j];
				calibrationdata.cameranormals_mat.at<double>(i, j) = calibrationdata.cameranormals[i][j];
				calibrationdata.velodynepoints_mat.at<double>(i, j) = calibrationdata.velodynepoints[i][j];
				calibrationdata.velodynenormals_mat.at<double>(i, j) = calibrationdata.velodynenormals[i][j];
				calibrationdata.velodynecorners_mat.at<double>(i, j) = calibrationdata.velodynecorners[i][j];
			}
			calibrationdata.pixeldata_mat.at<double>(i) = calibrationdata.pixeldata[i];
		}

		std::cout << "finish ";

		cv::Mat NN = calibrationdata.cameranormals_mat.t() * calibrationdata.cameranormals_mat;
		cv::Mat NM = calibrationdata.cameranormals_mat.t() * calibrationdata.velodynenormals_mat;
		cv::Mat UNR = (NN.inv() * NM).t(); // Analytical rotation matrix for real data
		std::cout << "Analytical rotation matrix " << UNR << std::endl;
		std::vector<double> euler;
		euler = rotm2eul(UNR); // rpy wrt original axes
		std::cout << "Analytical Euler angles " << euler.at(0) << " " << euler.at(1) << " " << euler.at(2) << " " << std::endl;
		eul.e1 = euler[0];
		eul.e2 = euler[1];
		eul.e3 = euler[2];

		EA::Chronometer timer;
		timer.tic();

		// Optimization for rotation alone
		GA_Type ga_obj;
		ga_obj.problem_mode = EA::GA_MODE::SOGA;
		ga_obj.multi_threading = false;
		ga_obj.verbose = false;
		ga_obj.population = 200;
		ga_obj.generation_max = 1000;
		ga_obj.calculate_SO_total_fitness = calculate_SO_total_fitness;
		ga_obj.init_genes = init_genes;
		ga_obj.eval_solution = eval_solution;
		ga_obj.mutate = mutate;
		ga_obj.crossover = crossover;
		ga_obj.SO_report_generation = SO_report_generation;
		ga_obj.best_stall_max = 100;
		ga_obj.average_stall_max = 100;
		ga_obj.tol_stall_average = 1e-8;
		ga_obj.tol_stall_best = 1e-8;
		ga_obj.elite_count = 10;
		ga_obj.crossover_fraction = 0.8;
		ga_obj.mutation_rate = 0.2;
		ga_obj.best_stall_max = 10;
		ga_obj.elite_count = 10;
		ga_obj.solve();

		// Optimized rotation
		tf::Matrix3x3 rot;
		rot.setRPY(eul_t.e1, eul_t.e2, eul_t.e3);
		cv::Mat tmp_rot = (cv::Mat_<double>(3, 3) << rot.getRow(0)[0], rot.getRow(0)[1], rot.getRow(0)[2],
						   rot.getRow(1)[0], rot.getRow(1)[1], rot.getRow(1)[2],
						   rot.getRow(2)[0], rot.getRow(2)[1], rot.getRow(2)[2]);
		// Analytical Translation
		cv::Mat cp_trans = tmp_rot * calibrationdata.camerapoints_mat.t();
		cv::Mat trans_diff = calibrationdata.velodynepoints_mat.t() - cp_trans;
		cv::Mat summed_diff;
		cv::reduce(trans_diff, summed_diff, 1, CV_REDUCE_SUM, CV_64F);
		summed_diff = summed_diff / trans_diff.cols;
		eul_t.x = summed_diff.at<double>(0);
		eul_t.y = summed_diff.at<double>(1);
		eul_t.z = summed_diff.at<double>(2);
		std::cout << "Rotation and Translation after first optimization " << eul_t.e1 << " "
				  << eul_t.e2 << " " << eul_t.e3 << " " << eul_t.x << " " << eul_t.y << " " << eul_t.z << std::endl;

		// extrinsics stored the vector of extrinsic parameters in every iteration
		std::vector<std::vector<double>> extrinsics;
		for (int i = 0; i < 10; i++)
		{
			// Joint optimization for Rotation and Translation (Perform this 10 times and take the average of the extrinsics)
			GA_Type2 ga_obj2;
			ga_obj2.problem_mode = EA::GA_MODE::SOGA;
			ga_obj2.multi_threading = false;
			ga_obj2.verbose = false;
			ga_obj2.population = 200;
			ga_obj2.generation_max = 1000;
			ga_obj2.calculate_SO_total_fitness = calculate_SO_total_fitness2;
			ga_obj2.init_genes = init_genes2;
			ga_obj2.eval_solution = eval_solution2;
			ga_obj2.mutate = mutate2;
			ga_obj2.crossover = crossover2;
			ga_obj2.SO_report_generation = SO_report_generation2;
			ga_obj2.best_stall_max = 100;
			ga_obj2.average_stall_max = 100;
			ga_obj2.tol_stall_average = 1e-8;
			ga_obj2.tol_stall_best = 1e-8;
			ga_obj2.elite_count = 10;
			ga_obj2.crossover_fraction = 0.8;
			ga_obj2.mutation_rate = 0.2;
			ga_obj2.best_stall_max = 10;
			ga_obj2.elite_count = 10;
			ga_obj2.solve();
			std::vector<double> ex_it;
			ex_it.push_back(eul_it.e1);
			ex_it.push_back(eul_it.e2);
			ex_it.push_back(eul_it.e3);
			ex_it.push_back(eul_it.x);
			ex_it.push_back(eul_it.y);
			ex_it.push_back(eul_it.z);
			extrinsics.push_back(ex_it);
			//      std::cout << "Extrinsics for iteration" << i << " " << extrinsics[i][0] << " " << extrinsics[i][1] << " " << extrinsics[i][2]
			//                << " " << extrinsics[i][3] << " " << extrinsics[i][4] << " " << extrinsics[i][5] << std::endl;
		}
		// Perform the average operation
		double e_x = 0.0;
		double e_y = 0.0;
		double e_z = 0.0;
		double e_e1 = 0.0;
		double e_e2 = 0.0;
		double e_e3 = 0.0;
		for (int i = 0; i < 10; i++)
		{
			e_e1 += extrinsics[i][0];
			e_e2 += extrinsics[i][1];
			e_e3 += extrinsics[i][2];
			e_x += extrinsics[i][3];
			e_y += extrinsics[i][4];
			e_z += extrinsics[i][5];
		}

		Rot_Trans rot_trans;
		rot_trans.e1 = e_e1 / 10;
		rot_trans.e2 = e_e2 / 10;
		rot_trans.e3 = e_e3 / 10;
		rot_trans.x = e_x / 10;
		rot_trans.y = e_y / 10;
		rot_trans.z = e_z / 10;
		std::cout << "Extrinsic Parameters "
				  << " " << rot_trans.e1 << " " << rot_trans.e2 << " " << rot_trans.e3
				  << " " << rot_trans.x << " " << rot_trans.y << " " << rot_trans.z << std::endl;
		std::cout << "The problem is optimized in " << timer.toc() << " seconds." << std::endl;

		Eigen::Vector3d euler_Eigen(rot_trans.e3, rot_trans.e2, rot_trans.e1); // 对应 z y x
		Eigen::Matrix3d rotation_matrix;
		rotation_matrix = Eigen::AngleAxisd(euler_Eigen[0], Eigen::Vector3d::UnitZ()) *
						  Eigen::AngleAxisd(euler_Eigen[1], Eigen::Vector3d::UnitY()) *
						  Eigen::AngleAxisd(euler_Eigen[2], Eigen::Vector3d::UnitX());

		cv::Mat lidar_to_cam_T = (cv::Mat_<double>(4, 4) << rotation_matrix(0, 0), rotation_matrix(0, 1), rotation_matrix(0, 2), rot_trans.x,
								  rotation_matrix(1, 0), rotation_matrix(1, 1), rotation_matrix(1, 2), rot_trans.y,
								  rotation_matrix(2, 0), rotation_matrix(2, 1), rotation_matrix(2, 2), rot_trans.z,
								  0, 0, 0, 1);

		
		time_t nowtime = time(NULL);
		struct tm *p;
		p = gmtime(&nowtime);
		std::string str_time;
		str_time = std::to_string(1900 + p->tm_year) + "-" + std::to_string(1 + p->tm_mon) + "-" + std::to_string(p->tm_mday) + "-" + std::to_string(8 + p->tm_hour) + "-" + std::to_string(p->tm_min);
		if (0 == mkdir(std::string(pkg_loc + "/calibration_result/" + str_time + "/").c_str(), 0755))
		{
			std::cout << "标定结果创建成功！" << std::endl;
		}
		else
		{
			std::cout << "文件夹创建失败！" << std::endl;
			return;
		}

		std::string str_cal_result_file = pkg_loc + "/calibration_result/" + str_time + "/calibration.yaml";
		cv::FileStorage calib_result(str_cal_result_file, cv::FileStorage::WRITE);
		calib_result << "camera_image_width" << i_params.image_size.first;
		calib_result << "camera_image_height" << i_params.image_size.second;
		calib_result << "camera_matrix" << i_params.cameramat;
		calib_result << "camera_distortion_coefficients" << i_params.distcoeff;
		calib_result << "lidar_camera_extrinsic" << lidar_to_cam_T;

		calib_result.release();

		image_projection(rot_trans); // Project the pointcloud on the image with the obtained extrinsics
	}								 // runs if you press 'o'
}

int main(int argc, char **argv)
{
	ROS_INFO("optimizer");
	ros::init(argc, argv, "optimizer");
	ros::NodeHandle n;

	int cb_l, cb_b, l, b, e_l, e_b, i_l, i_b;
	pkg_loc = ros::package::getPath("cam_lidar_calibration");
	std::string str_initial_file = pkg_loc + "/cfg/initial_params.yaml";

	cv::FileStorage fs(str_initial_file, cv::FileStorage::READ | cv::FileStorage::FORMAT_YAML);
	if (!fs.isOpened())
	{
		std::cout << " [ " + str_initial_file + " ] 文件打开失败" << std::endl;
		return 0;
	}

	i_params.camera_topic = std::string(fs["image_topic"]);
	i_params.lidar_topic = std::string(fs["pointcloud_topic"]);
	i_params.fisheye_model = int(fs["distortion_model"]);
	i_params.lidar_ring_count = fs["scan_line"];
	i_params.grid_size = std::make_pair(int(fs["chessboard"]["length"]), int(fs["chessboard"]["width"]));
	i_params.square_length = fs["chessboard"]["grid_size"];
	i_params.board_dimension = std::make_pair(int(fs["board_dimension"]["length"]), int(fs["board_dimension"]["width"]));
	i_params.cb_translation_error = std::make_pair(int(fs["translation_error"]["length"]), int(fs["translation_error"]["width"]));
	fs["camera_matrix"] >> i_params.cameramat;
	i_params.distcoeff_num = 5;
	fs["distortion_coefficients"] >> i_params.distcoeff;

	i_params.image_size = std::make_pair(int(fs["image_pixel"]["width"]), int(fs["image_pixel"]["height"]));


	ros::Subscriber flag_sub = n.subscribe("/flag", 5, my_flag);
	ros::Subscriber calibdata_sub = n.subscribe("/feature_extraction/roi/points", 5, get_samples);

	message_filters::Subscriber<sensor_msgs::Image> image_sub(n, i_params.camera_topic, 5);
	message_filters::Subscriber<sensor_msgs::PointCloud2> pcl_sub(n, i_params.lidar_topic, 5);
	typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::PointCloud2> MySyncPolicy;
	message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(5), image_sub, pcl_sub);
	sync.registerCallback(boost::bind(&sensor_info_callback, _1, _2));

	image_transport::ImageTransport it(n);
	pub_img_dist = it.advertise("image_projection", 20);

	ros::spin();
	return 0;
}
// Function converts rotation matrix to corresponding euler angles
std::vector<double> rotm2eul(cv::Mat mat)
{
	std::vector<double> euler(3);
	euler[0] = atan2(mat.at<double>(2, 1), mat.at<double>(2, 2)); // rotation about x axis: roll
	euler[1] = atan2(-mat.at<double>(2, 0), sqrt(mat.at<double>(2, 1) * mat.at<double>(2, 1) + mat.at<double>(2, 2) * mat.at<double>(2, 2)));
	euler[2] = atan2(mat.at<double>(1, 0), mat.at<double>(0, 0)); // rotation about z axis: yaw
	return euler;
}
double tmpxC;
double *converto_imgpts(double x, double y, double z)
{
	tmpxC = x / z;
	double tmpyC = y / z;
	cv::Point2d planepointsC;

	planepointsC.x = tmpxC;
	planepointsC.y = tmpyC;

	double r2 = tmpxC * tmpxC + tmpyC * tmpyC;

	if (i_params.fisheye_model)
	{
		double r1 = pow(r2, 0.5);
		double a0 = std::atan(r1);
		double a1 = a0 * (1 + i_params.distcoeff.at<double>(0) * pow(a0, 2) + i_params.distcoeff.at<double>(1) * pow(a0, 4) + i_params.distcoeff.at<double>(2) * pow(a0, 6) + i_params.distcoeff.at<double>(3) * pow(a0, 8));
		planepointsC.x = (a1 / r1) * tmpxC;
		planepointsC.y = (a1 / r1) * tmpyC;
		planepointsC.x = i_params.cameramat.at<double>(0, 0) * planepointsC.x + i_params.cameramat.at<double>(0, 2);
		planepointsC.y = i_params.cameramat.at<double>(1, 1) * planepointsC.y + i_params.cameramat.at<double>(1, 2);
	}
	else // For pinhole camera model
	{
		double tmpdist = 1 + i_params.distcoeff.at<double>(0) * r2 + i_params.distcoeff.at<double>(1) * r2 * r2 +
						 i_params.distcoeff.at<double>(4) * r2 * r2 * r2;
		planepointsC.x = tmpxC * tmpdist + 2 * i_params.distcoeff.at<double>(2) * tmpxC * tmpyC +
						 i_params.distcoeff.at<double>(3) * (r2 + 2 * tmpxC * tmpxC);
		planepointsC.y = tmpyC * tmpdist + i_params.distcoeff.at<double>(2) * (r2 + 2 * tmpyC * tmpyC) +
						 2 * i_params.distcoeff.at<double>(3) * tmpxC * tmpyC;
		planepointsC.x = i_params.cameramat.at<double>(0, 0) * planepointsC.x + i_params.cameramat.at<double>(0, 2);
		planepointsC.y = i_params.cameramat.at<double>(1, 1) * planepointsC.y + i_params.cameramat.at<double>(1, 2);
	}

	double *img_coord = new double[2];
	*(img_coord) = planepointsC.x;
	*(img_coord + 1) = planepointsC.y;

	return img_coord;
}

void sensor_info_callback(const sensor_msgs::Image::ConstPtr &img, const sensor_msgs::PointCloud2::ConstPtr &pc)
{
	if (!sensor_pair) // Take the first synchronized camera-lidar pair from the input
	{
		cv_bridge::CvImagePtr cv_ptr;
		try
		{
			cv_ptr = cv_bridge::toCvCopy(img, "bgr8");
		}
		catch (cv_bridge::Exception &e)
		{
			return;
		}
		raw_image = cv_ptr->image;

		pcl::fromROSMsg(*pc, *cloud);
		sensor_pair = 1;
	}
}

void image_projection(Rot_Trans rot_trans)
{

	cv::Mat new_image_raw;
	new_image_raw = raw_image.clone();

	// Extrinsic parameter: Transform Velodyne -> cameras
	tf::Matrix3x3 rot;
	rot.setRPY(rot_trans.e1, rot_trans.e2, rot_trans.e3);

	Eigen::MatrixXf t1(4, 4), t2(4, 4);
	t1 << rot.getRow(0)[0], rot.getRow(0)[1], rot.getRow(0)[2], rot_trans.x,
		rot.getRow(1)[0], rot.getRow(1)[1], rot.getRow(1)[2], rot_trans.y,
		rot.getRow(2)[0], rot.getRow(2)[1], rot.getRow(2)[2], rot_trans.z,
		0, 0, 0, 1;
	t2 = t1.inverse();

	Eigen::Affine3f transform_A = Eigen::Affine3f::Identity();
	transform_A.matrix() << t2(0, 0), t2(0, 1), t2(0, 2), t2(0, 3),
		t2(1, 0), t2(1, 1), t2(1, 2), t2(1, 3),
		t2(2, 0), t2(2, 1), t2(2, 2), t2(2, 3),
		t2(3, 0), t2(3, 1), t2(3, 2), t2(3, 3);

	if (cloud->size() < 1)
		return;

	pcl::PointCloud<pcl::PointXYZIR> organized;
	organized = organized_pointcloud(cloud);

	for (pcl::PointCloud<pcl::PointXYZIR>::const_iterator it = organized.begin(); it != organized.end(); it++)
	{
		pcl::PointXYZIR itA;
		itA = pcl::transformPoint(*it, transform_A);
		if (itA.z < 0 or std::abs(itA.x / itA.z) > 1.2)
			continue;

		double *img_pts = converto_imgpts(itA.x, itA.y, itA.z);
		double length = sqrt(pow(itA.x, 2) + pow(itA.y, 2) + pow(itA.z, 2)); // range of every point
		int color = std::min(round((length / 30) * 49), 49.0);

		if (img_pts[1] >= 0 and img_pts[1] < i_params.image_size.second and img_pts[0] >= 0 and img_pts[0] < i_params.image_size.first)
		{
			cv::circle(new_image_raw, cv::Point(img_pts[0], img_pts[1]), 3,
					   CV_RGB(255 * colmap[color][0], 255 * colmap[color][1], 255 * colmap[color][2]), -1);
		}
	}

	// Publish the image projection
	ros::Time time = ros::Time::now();
	cv_ptr->encoding = "bgr8";
	cv_ptr->header.stamp = time;
	cv_ptr->header.frame_id = "/traj_output";
	cv_ptr->image = new_image_raw;
	pub_img_dist.publish(cv_ptr->toImageMsg());
}

pcl::PointCloud<pcl::PointXYZIR> organized_pointcloud(pcl::PointCloud<pcl::PointXYZIR>::Ptr input_pointcloud)
{
	pcl::PointCloud<pcl::PointXYZIR> organized_pc;
	pcl::KdTreeFLANN<pcl::PointXYZIR> kdtree;

	// Kdtree to sort the point cloud
	kdtree.setInputCloud(input_pointcloud);

	pcl::PointXYZIR searchPoint; // camera position as target
	searchPoint.x = 0.0f;
	searchPoint.y = 0.0f;
	searchPoint.z = 0.0f;

	int K = input_pointcloud->points.size();
	std::vector<int> pointIdxNKNSearch(K);
	std::vector<float> pointNKNSquaredDistance(K);

	// Sort the point cloud based on distance to the camera
	if (kdtree.nearestKSearch(searchPoint, K, pointIdxNKNSearch, pointNKNSquaredDistance) > 0)
	{
		for (size_t i = 0; i < pointIdxNKNSearch.size(); ++i)
		{
			pcl::PointXYZIR point;
			point.x = input_pointcloud->points[pointIdxNKNSearch[i]].x;
			point.y = input_pointcloud->points[pointIdxNKNSearch[i]].y;
			point.z = input_pointcloud->points[pointIdxNKNSearch[i]].z;
			point.intensity = input_pointcloud->points[pointIdxNKNSearch[i]].intensity;
			point.ring = input_pointcloud->points[pointIdxNKNSearch[i]].ring;
			organized_pc.push_back(point);
		}
	}

	// Return sorted point cloud
	return (organized_pc);
}
