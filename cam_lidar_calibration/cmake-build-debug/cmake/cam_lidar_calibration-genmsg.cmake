# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "cam_lidar_calibration: 2 messages, 0 services")

set(MSG_I_FLAGS "-Icam_lidar_calibration:/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg;-Istd_msgs:/opt/ros/melodic/share/std_msgs/cmake/../msg;-Isensor_msgs:/opt/ros/melodic/share/sensor_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/melodic/share/geometry_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(cam_lidar_calibration_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/calibration_data.msg" NAME_WE)
add_custom_target(_cam_lidar_calibration_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "cam_lidar_calibration" "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/calibration_data.msg" ""
)

get_filename_component(_filename "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/extrinsics.msg" NAME_WE)
add_custom_target(_cam_lidar_calibration_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "cam_lidar_calibration" "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/extrinsics.msg" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(cam_lidar_calibration
  "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/calibration_data.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/cam_lidar_calibration
)
_generate_msg_cpp(cam_lidar_calibration
  "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/extrinsics.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/cam_lidar_calibration
)

### Generating Services

### Generating Module File
_generate_module_cpp(cam_lidar_calibration
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/cam_lidar_calibration
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(cam_lidar_calibration_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(cam_lidar_calibration_generate_messages cam_lidar_calibration_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/calibration_data.msg" NAME_WE)
add_dependencies(cam_lidar_calibration_generate_messages_cpp _cam_lidar_calibration_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/extrinsics.msg" NAME_WE)
add_dependencies(cam_lidar_calibration_generate_messages_cpp _cam_lidar_calibration_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(cam_lidar_calibration_gencpp)
add_dependencies(cam_lidar_calibration_gencpp cam_lidar_calibration_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS cam_lidar_calibration_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(cam_lidar_calibration
  "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/calibration_data.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/cam_lidar_calibration
)
_generate_msg_eus(cam_lidar_calibration
  "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/extrinsics.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/cam_lidar_calibration
)

### Generating Services

### Generating Module File
_generate_module_eus(cam_lidar_calibration
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/cam_lidar_calibration
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(cam_lidar_calibration_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(cam_lidar_calibration_generate_messages cam_lidar_calibration_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/calibration_data.msg" NAME_WE)
add_dependencies(cam_lidar_calibration_generate_messages_eus _cam_lidar_calibration_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/extrinsics.msg" NAME_WE)
add_dependencies(cam_lidar_calibration_generate_messages_eus _cam_lidar_calibration_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(cam_lidar_calibration_geneus)
add_dependencies(cam_lidar_calibration_geneus cam_lidar_calibration_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS cam_lidar_calibration_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(cam_lidar_calibration
  "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/calibration_data.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/cam_lidar_calibration
)
_generate_msg_lisp(cam_lidar_calibration
  "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/extrinsics.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/cam_lidar_calibration
)

### Generating Services

### Generating Module File
_generate_module_lisp(cam_lidar_calibration
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/cam_lidar_calibration
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(cam_lidar_calibration_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(cam_lidar_calibration_generate_messages cam_lidar_calibration_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/calibration_data.msg" NAME_WE)
add_dependencies(cam_lidar_calibration_generate_messages_lisp _cam_lidar_calibration_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/extrinsics.msg" NAME_WE)
add_dependencies(cam_lidar_calibration_generate_messages_lisp _cam_lidar_calibration_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(cam_lidar_calibration_genlisp)
add_dependencies(cam_lidar_calibration_genlisp cam_lidar_calibration_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS cam_lidar_calibration_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(cam_lidar_calibration
  "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/calibration_data.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/cam_lidar_calibration
)
_generate_msg_nodejs(cam_lidar_calibration
  "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/extrinsics.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/cam_lidar_calibration
)

### Generating Services

### Generating Module File
_generate_module_nodejs(cam_lidar_calibration
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/cam_lidar_calibration
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(cam_lidar_calibration_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(cam_lidar_calibration_generate_messages cam_lidar_calibration_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/calibration_data.msg" NAME_WE)
add_dependencies(cam_lidar_calibration_generate_messages_nodejs _cam_lidar_calibration_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/extrinsics.msg" NAME_WE)
add_dependencies(cam_lidar_calibration_generate_messages_nodejs _cam_lidar_calibration_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(cam_lidar_calibration_gennodejs)
add_dependencies(cam_lidar_calibration_gennodejs cam_lidar_calibration_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS cam_lidar_calibration_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(cam_lidar_calibration
  "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/calibration_data.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/cam_lidar_calibration
)
_generate_msg_py(cam_lidar_calibration
  "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/extrinsics.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/cam_lidar_calibration
)

### Generating Services

### Generating Module File
_generate_module_py(cam_lidar_calibration
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/cam_lidar_calibration
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(cam_lidar_calibration_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(cam_lidar_calibration_generate_messages cam_lidar_calibration_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/calibration_data.msg" NAME_WE)
add_dependencies(cam_lidar_calibration_generate_messages_py _cam_lidar_calibration_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/extrinsics.msg" NAME_WE)
add_dependencies(cam_lidar_calibration_generate_messages_py _cam_lidar_calibration_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(cam_lidar_calibration_genpy)
add_dependencies(cam_lidar_calibration_genpy cam_lidar_calibration_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS cam_lidar_calibration_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/cam_lidar_calibration)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/cam_lidar_calibration
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(cam_lidar_calibration_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()
if(TARGET sensor_msgs_generate_messages_cpp)
  add_dependencies(cam_lidar_calibration_generate_messages_cpp sensor_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/cam_lidar_calibration)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/cam_lidar_calibration
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(cam_lidar_calibration_generate_messages_eus std_msgs_generate_messages_eus)
endif()
if(TARGET sensor_msgs_generate_messages_eus)
  add_dependencies(cam_lidar_calibration_generate_messages_eus sensor_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/cam_lidar_calibration)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/cam_lidar_calibration
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(cam_lidar_calibration_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()
if(TARGET sensor_msgs_generate_messages_lisp)
  add_dependencies(cam_lidar_calibration_generate_messages_lisp sensor_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/cam_lidar_calibration)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/cam_lidar_calibration
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(cam_lidar_calibration_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()
if(TARGET sensor_msgs_generate_messages_nodejs)
  add_dependencies(cam_lidar_calibration_generate_messages_nodejs sensor_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/cam_lidar_calibration)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/cam_lidar_calibration\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/cam_lidar_calibration
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(cam_lidar_calibration_generate_messages_py std_msgs_generate_messages_py)
endif()
if(TARGET sensor_msgs_generate_messages_py)
  add_dependencies(cam_lidar_calibration_generate_messages_py sensor_msgs_generate_messages_py)
endif()
