cmake_minimum_required(VERSION 3.15)
project(camera_calibration)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(corners_finder corners_finder.cpp)
target_link_libraries(corners_finder ${OpenCV_LIBS})

add_executable(camera_calibration camera_calibration.cpp)
target_link_libraries(camera_calibration ${OpenCV_LIBS})

add_executable(undistortion undistortion.cpp)
target_link_libraries(undistortion ${OpenCV_LIBS})