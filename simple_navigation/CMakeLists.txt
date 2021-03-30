cmake_minimum_required(VERSION 3.0.2)
project(simple_navigation)

find_package(catkin REQUIRED COMPONENTS
  actionlib
  move_base_msgs
  std_msgs
  simple_navigation_service_msg
  rospy
)

catkin_package(
  CATKIN_DEPENDS 
  rospy
  simple_navigation_service_msg
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)
