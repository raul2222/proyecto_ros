#!/usr/bin/env python
import rospy
import cv2
import numpy as np
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import Image
import searchFace

class Ros2OpenCVImageConverter():   

    def __init__(self):
    
        self.bridge_object = CvBridge()
        #/raspicam_node/image para real
        self.image_sub = rospy.Subscriber("raspicam_node/image",Image,self.camera_callback)
        self.face_cascade = cv2.CascadeClassifier('/home/raul/catkin_ws/src/image_capture/src/clasificadores/haarcascade_frontalface_default.xml')
        self.body_cascade = cv2.CascadeClassifier('/home/raul/catkin_ws/src/image_capture/src/clasificadores/haarcascade_fullbody.xml')
        self.n = 0

    def camera_callback(self,data):

        try:
            # Seleccionamos bgr8 porque es la codificacion de OpenCV por defecto
            cv_image = self.bridge_object.imgmsg_to_cv2(data, desired_encoding="bgr8")
        except CvBridgeError as e:
            print(e)
        img_gray = cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)
        
        
        try:
            detections = self.face_cascade.detectMultiScale(img_gray, 1.1, 1)
            for (x,y,w,h) in detections:
                self.n += 1
   
                roi_color = cv_image[y:y+h, x:x+w]
   
                cv2.imwrite('face'+str(self.n)+'.jpg', roi_color)
                searchFace.upload_file('face'+str(self.n)+'.jpg', "rekpractica567", object_name=None)
                cara = searchFace.searchFace("rekpractica567",'face'+str(self.n)+'.jpg',"gente")
                if cara!= "None":
                    if cara== "Intruder":
                        cv2.rectangle(cv_image,(x,y),(x+w,y+h),(0,0,255),2)
                        cv2.putText(cv_image, "Intruso", (x, y+h+10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,0,0),2)
                        print(cara)
                    else:
                        if cara!=None:
                            cv2.rectangle(cv_image,(x,y),(x+w,y+h),(0,255,0),2)
                            cv2.putText(cv_image, cara, (x, y+h+10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,0,0),2)

        except:
            print('No detections')
        
        
        cv2.imshow("Imagen capturada por el robot", cv_image)
                
        cv2.waitKey(1)    

def main():    
    img_converter_object = Ros2OpenCVImageConverter()
    rospy.init_node('Ros2OpenCVImageConverter', anonymous=True)
    
    try:
        rospy.spin()
    except KeyboardInterrupt:
        print("Fin del programa!")
    
    cv2.destroyAllWindows() 
    

if __name__ == '__main__':
    main()
