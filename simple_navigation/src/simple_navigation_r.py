#!/usr/bin/env python

import rospy
import actionlib
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from simple_navigation_service_msg.srv import SimpleNavigationServiceMessage, SimpleNavigationServiceMessageResponse

"""Points = {
    1: (15.9, -3.7, 6.0),
    2: (15.0, -3.3, 4.25),
    3: (15.0, -4.0, 6.0),
    4: (15.8, -4.5, 1.0),
    5: (15.7, -5.1, 2.7),
    6: (14.8, -4.8, 2.7)
}"""

Points = {
    1: (15.9, -3.7, 0.0),
    2: (15.0, -3.3, -0.72),
    3: (15.0, -4.0, 0.0),
    4: (15.8, -4.5, 0.72),
    5: (15.7, -5.1, 1.0),
    6: (14.8, -4.8, 1.0)
}

"""
1: (0.9, 0.35, 3.14), 
    2: (-0.1, -0.1, 1.57), 
    3: (1.0, -0.5, -1.57),
    4: (-0.2 ,-1.75, 3.14),"""

"""1: (-1.0, -0.8, 3.14), 
    2: (-1.5, 1.0, 1.57), 
    3: (-0.5, 1.5, -1.57)"""

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
