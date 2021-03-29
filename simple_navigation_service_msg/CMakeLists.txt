cmake_minimum_required(VERSION 3.0.2)
project(simple_navigation_service_msg)


find_package(catkin REQUIRED COMPONENTS
  rospy
  std_msgs
  message_generation
)


add_service_files(
  FILES
  SimpleNavigationServiceMessage.srv
)


generate_messages(
  DEPENDENCIES
  std_msgs  # Or other packages containing msgs
)


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES simple_navigation_service_msg
 CATKIN_DEPENDS rospy
#  DEPENDS system_lib
)


include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)


