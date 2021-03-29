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
    """Recibe la petición de un servicio y asigna un goal al cliente move_base

    Args:
        request: los parametros del mensaje del servicio

    Returns:
        bool: la respuesta del servicio
    """

    # Creamos un cliente de accion con la accion MoveBaseAction
    client = actionlib.SimpleActionClient('move_base',MoveBaseAction)

    # Esperamos a que el servidor empiece y espere goals
    client.wait_for_server()

    # Creamos un goal con el constructor MoveBaseGoal
    goal = MoveBaseGoal()
    goal.target_pose.header.frame_id = "map"
    goal.target_pose.header.stamp = rospy.Time.now()

    # Si no existe el punto recibido enviamos un error devolvemos la respuesta del servicio como False 
    if Points.get(request.point) == None:
        rospy.logerr("El punto tiene que ser del 1 al 6")
        response = SimpleNavigationServiceMessageResponse()
        response.success = False
        return response
 
    # Asignamos las coordenadas y orientacion del goal
    goal.target_pose.pose.position.x = Points[request.point][0]
    goal.target_pose.pose.position.y = Points[request.point][1]
    goal.target_pose.pose.orientation.z = Points[request.point][2]
    goal.target_pose.pose.orientation.w = 1.0

    # Enviamos el goal al servidor
    client.send_goal(goal)

    # Esperamos a que el servidor acabe la accion
    wait = client.wait_for_result()

    # Si el resultado no llega, asumimos que el servidor no esta disponible
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

rospy.init_node('movebase_client_py') # Inicializamos nodo
service = rospy.Service('/simple_navigation', SimpleNavigationServiceMessage, movebase_client) # Inicializamos servicio
rospy.spin() # Dejamos el script en marcha

