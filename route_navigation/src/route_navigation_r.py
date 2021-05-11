#!/usr/bin/env python
import rospy
import smach
import time
from smach import State, StateMachine
from smach_ros import IntrospectionServer

import actionlib
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal, MoveBaseActionFeedback
from route_navigation_service_msg.srv import RouteNavigationServiceMessage, RouteNavigationServiceMessageResponse
from tf.transformations import quaternion_from_euler
from collections import OrderedDict


## Puntos de la casa
waypoints = [
    ['GO_TO_SECTOR1', (-1.0, -0.8, 0.0), (0.0, 0.0, -1.57, 0.0)],
    ['GO_TO_SECTOR2', (-1.5, -1.0, 0.0), (0.0, 0.0, 3.14, 0.0)],
    ['GO_TO_SECTOR3',(-1.5, 0.5, 0.0), (0.0, 0.0, 1.57, 0.0)]
]



## Descripcion de los estados
class PowerOnRobot(State):
    def __init__(self):
        State.__init__(self, outcomes=['1','0'])

    def execute(self, userdata):
        rospy.loginfo("Encendiendo el Robot")
        time.sleep(2)
        return '1'



class Navigate(State):
    def __init__(self, position, orientation, place):
        State.__init__(self, outcomes=['1', '0'], input_keys=[''], output_keys=[''])
        self._position =position
        self._orientation =orientation
        self._place = place
        self._move_base = actionlib.SimpleActionClient("/move_base", MoveBaseAction)
        rospy.loginfo("Activando el cliente de navegacion..")
        self._move_base.wait_for_server(rospy.Duration(15))


    def execute(self, userdata):
        time.sleep(2)
        goal = MoveBaseGoal()
        goal.target_pose.header.frame_id = 'map'
        rospy.loginfo(self._position)
        goal.target_pose.pose.position.x = self._position[0]
        goal.target_pose.pose.position.y = self._position[1]
        goal.target_pose.pose.position.z = self._position[2]
        goal.target_pose.pose.orientation.x = self._orientation[0]
        goal.target_pose.pose.orientation.y = self._orientation[1]
        goal.target_pose.pose.orientation.z = self._orientation[2]
        goal.target_pose.pose.orientation.w = self._orientation[3]

        rospy.loginfo("ROBOT %s" %(self._place))
        # sends the goal
        self._move_base.send_goal(goal)
        self._move_base.wait_for_result()
        # Comprobamos el estado de la navegacion
        nav_state = self._move_base.get_state()
        rospy.loginfo("[Result] State: %d" % (nav_state))
        nav_state = 3

        if nav_state == 3:
            return '1'
        else:
            return '0'



class main():
    def __init__(self, request):

        sm= StateMachine(outcomes=['succeeded', 'aborted'])
        sm.userdata.sm_input = 1

        with sm:
            StateMachine.add('POWER_ON', PowerOnRobot(), transitions={'1':waypoints[0][0], '0':'aborted'})
            StateMachine.add(waypoints[0][0], Navigate( waypoints[0][1], waypoints[0][2], waypoints[0][0]), transitions={'1':waypoints[1][0],'0':'aborted'})
            StateMachine.add(waypoints[1][0], Navigate( waypoints[1][1], waypoints[1][2], waypoints[1][0]), transitions={'1':waypoints[2][0],'0':'aborted'})
            StateMachine.add(waypoints[2][0], Navigate( waypoints[2][1], waypoints[2][2], waypoints[2][0]), transitions={'1':waypoints[0][0],'0':'aborted'})
            
        intro_server = IntrospectionServer('Robot_vigilante',sm, '/SM_ROOT')
        intro_server.start()

        #Ejecutamos la maquina de estados
        sm_outcome = sm.execute()
        intro_server.stop()


    def shutdown(self):
        rospy.loginf("Parando la ejecucion...")
        rospy.sleep(1)

if __name__=='__main__':
    try:
        rospy.init_node('move_base_action_client', anonymous=False)
        service = rospy.Service('/route_navigation', RouteNavigationServiceMessage, main)
        rospy.spin()
    except rospy.ROSInterruptException:
        rospy.loginfo(" Testeo Robot Vigilante finalizado")