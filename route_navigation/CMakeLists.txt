cmake_minimum_required(VERSION 3.0.2)
project(route_navigation)

find_package(catkin REQUIRED COMPONENTS
  rospy
  smach_ros
  actionlib
  move_base_msgs
  std_msgs
  route_navigation_service_msg
)

catkin_package(
  CATKIN_DEPENDS 
  rospy
  route_navigation_service_msg
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)
