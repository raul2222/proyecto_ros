#!/usr/bin/env python

import rospy
import actionlib
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from simple_navigation_service_msg.srv import SimpleNavigationServiceMessage, SimpleNavigationServiceMessageResponse

Points = {
    1: (-2.0, 0.25, 3.14), 
    2: (-1.5, 2.2, 1.57), 
    3: (-0.5, -0.6, -1.57),
    4: (3.5, -0.6, -1.57),
    5: (3.5, -2.88, 0.0),
    6: (3.6, 2.6, 1.57)
}

def movebase_client(request):
    client = actionlib.SimpleActionClient('move_base',MoveBaseAction)
    client.wait_for_server()

    goal = MoveBaseGoal()
    goal.target_pose.header.frame_id = "map"
    goal.target_pose.header.stamp = rospy.Time.now()

    if Points.get(request.point) == None:
        rospy.logerr("El punto tiene que ser del 1 al 6")
        response = SimpleNavigationServiceMessageResponse()
        response.success = False
        return response
 
    goal.target_pose.pose.position.x = Points[request.point][0]
    goal.target_pose.pose.position.y = Points[request.point][1]
    goal.target_pose.pose.orientation.z = Points[request.point][2]
    goal.target_pose.pose.orientation.w = 1.0

    client.send_goal(goal)
    wait = client.wait_for_result()
    if not wait:
        rospy.logerr("Servidor de accion no disponible")
        response = SimpleNavigationServiceMessageResponse()
        response.success = False
        return response
        rospy.signal_shutdown("Servidor de accion no disponible")
    else:
        rospy.loginfo("Goal conseguido")
        response = SimpleNavigationServiceMessageResponse()
        response.success = True
        return response
        #return client.get_result()   

rospy.init_node('movebase_client_py')
service = rospy.Service('/simple_navigation', SimpleNavigationServiceMessage, movebase_client)
rospy.spin()

"""
try:
    rospy.init_node('movebase_client_py')
    result = movebase_client()
    if result:
        rospy.loginfo("Goal hecho")
except rospy.ROSInterruptException:
    rospy.loginfo("Navegacion finalizada.")
"""
