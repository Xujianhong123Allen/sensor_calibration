cmake_minimum_required(VERSION 3.0.2)
project(handeye_calibration)


add_compile_options(-std=c++14)


find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  roscpp
  tf
  sensor_msgs
  message_filters
  image_transport
)

set(OpenCV_DIR "/usr/local/share/OpenCV")
#set(OpenCV_DIR "/usr/share/OpenCV")
find_package(OpenCV REQUIRED)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES calibration
#  CATKIN_DEPENDS cv_bridge roscpp tf
#  DEPENDS system_lib
)

###########
## Build ##
###########

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)



# 采集手眼标定数据
add_executable(collect_hand_eye_data  src/collect_hand_eye_data.cpp)
target_link_libraries(collect_hand_eye_data  ${catkin_LIBRARIES}  ${OpenCV_LIBRARIES})

## 4. 手眼标定程序
add_executable(calibrate_hand_eye  src/calibrate_hand_eye.cpp)
target_link_libraries(calibrate_hand_eye  ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

