cmake_minimum_required(VERSION 3.0.2)
project(test)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)
catkin_package(
  INCLUDE_DIRS
    include
  LIBRARIES
    test
  CATKIN_DEPENDS
    roscpp
    rospy
    std_msgs
  DEPENDS
    system_lib
)


include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)
add_executable(pub src/pub.cpp)
target_link_libraries(pub ${catkin_LIBRARIES} ${PROJECT_SOURCE_DIR}/lib/libvdsrc.so)
