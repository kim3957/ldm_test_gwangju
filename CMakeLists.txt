cmake_minimum_required(VERSION 3.0.2)
project(test)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
  )
add_message_files(
  FILES
  LDM.msg
  )

generate_messages(
  DEPENDENCIES
  std_msgs)
catkin_package(
  INCLUDE_DIRS
    include
  LIBRARIES
    test
  CATKIN_DEPENDS
    roscpp
    rospy
    std_msgs
    message_runtime
  DEPENDS
    system_lib
  EXPORTED_TARGETS
    test_generate_messages_cpp
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  )

add_executable(pub src/pub.cpp)
target_link_libraries(pub ${catkin_LIBRARIES} ${PROJECT_SOURCE_DIR}/lib/libvdsrc.so)
add_executable(recv src/recv.cc)
add_dependencies(recv test_generate_messages_cpp)
target_link_libraries(recv ${catkin_LIBRARIES}
  ${PROJECT_SOURCE_DIR}/lib/libvdsrc.so
  )
link_directories(/home/kim/catkin_ws/src/test/include/test)



