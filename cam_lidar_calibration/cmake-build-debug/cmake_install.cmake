# Install script for directory: /media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  
      if (NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}")
        file(MAKE_DIRECTORY "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}")
      endif()
      if (NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/.catkin")
        file(WRITE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/.catkin" "")
      endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/_setup_util.py")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local" TYPE PROGRAM FILES "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/catkin_generated/installspace/_setup_util.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/env.sh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local" TYPE PROGRAM FILES "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/catkin_generated/installspace/env.sh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/setup.bash;/usr/local/local_setup.bash")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local" TYPE FILE FILES
    "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/catkin_generated/installspace/setup.bash"
    "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/catkin_generated/installspace/local_setup.bash"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/setup.sh;/usr/local/local_setup.sh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local" TYPE FILE FILES
    "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/catkin_generated/installspace/setup.sh"
    "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/catkin_generated/installspace/local_setup.sh"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/setup.zsh;/usr/local/local_setup.zsh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local" TYPE FILE FILES
    "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/catkin_generated/installspace/setup.zsh"
    "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/catkin_generated/installspace/local_setup.zsh"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/.rosinstall")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local" TYPE FILE FILES "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/catkin_generated/installspace/.rosinstall")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cam_lidar_calibration/msg" TYPE FILE FILES
    "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/calibration_data.msg"
    "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/msg/extrinsics.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cam_lidar_calibration/cmake" TYPE FILE FILES "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/catkin_generated/installspace/cam_lidar_calibration-msg-paths.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/devel/include/cam_lidar_calibration")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/roseus/ros" TYPE DIRECTORY FILES "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/devel/share/roseus/ros/cam_lidar_calibration")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/common-lisp/ros" TYPE DIRECTORY FILES "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/devel/share/common-lisp/ros/cam_lidar_calibration")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gennodejs/ros" TYPE DIRECTORY FILES "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/devel/share/gennodejs/ros/cam_lidar_calibration")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND "/usr/bin/python2" -m compileall "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/devel/lib/python2.7/dist-packages/cam_lidar_calibration")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages" TYPE DIRECTORY FILES "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/devel/lib/python2.7/dist-packages/cam_lidar_calibration")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cam_lidar_calibration" TYPE FILE FILES "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/devel/include/cam_lidar_calibration/boundsConfig.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages/cam_lidar_calibration" TYPE FILE FILES "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/devel/lib/python2.7/dist-packages/cam_lidar_calibration/__init__.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND "/usr/bin/python2" -m compileall "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/devel/lib/python2.7/dist-packages/cam_lidar_calibration/cfg")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages/cam_lidar_calibration" TYPE DIRECTORY FILES "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/devel/lib/python2.7/dist-packages/cam_lidar_calibration/cfg")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/catkin_generated/installspace/cam_lidar_calibration.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cam_lidar_calibration/cmake" TYPE FILE FILES "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/catkin_generated/installspace/cam_lidar_calibration-msg-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cam_lidar_calibration/cmake" TYPE FILE FILES
    "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/catkin_generated/installspace/cam_lidar_calibrationConfig.cmake"
    "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/catkin_generated/installspace/cam_lidar_calibrationConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cam_lidar_calibration" TYPE FILE FILES "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/package.xml")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/gtest/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/media/ch/fdff4d85-b8f7-445b-905b-5ce418a00fc9/ch/NutstoreFiles/MyCloud/ExternalBrain/C2_ToBeProcessed/MyResearchingProject/lidar/catkin_velodyne/src/cam_lidar_calibration/cmake-build-debug/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
