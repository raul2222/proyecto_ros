/*
   rosserial Ultrasound Example

   This example is for the HC-SR04 Ultrasound sensor
*/

#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>

ros::NodeHandle  nh;

sensor_msgs::Range range_msg;
ros::Publisher pub_range( "/ultrasound", &range_msg);

const int trigger_pin = 7;
const int echo_pin = 6;

char frameid[] = "/ultrasound";

float microsecondsToCentimeters(float microseconds) {
  return microseconds / 58.3;
}
float getRange_Ultrasound(int echo_pin, int trigger_pin) {
  float duration, cm;
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);
  duration = pulseIn(echo_pin, HIGH);
  cm = microsecondsToCentimeters(duration);
  return cm;
}

void setup()
{
  nh.initNode();
  nh.advertise(pub_range);


  range_msg.radiation_type = sensor_msgs::Range::ULTRASOUND;
  range_msg.header.frame_id =  frameid;
  range_msg.field_of_view = 0.1;  // fake
  range_msg.min_range = 0.0;
  range_msg.max_range = 6.47;

  pinMode(trigger_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
}


long range_time;

void loop()
{

  //publish the echo value every 50 milliseconds
  //since it takes that long for the sensor to stablize
  if ( millis() >= range_time ) {
    int r = 0;

    range_msg.range = getRange_Ultrasound(echo_pin, trigger_pin);
    range_msg.header.stamp = nh.now();
    pub_range.publish(&range_msg);
    range_time =  millis() + 50;
  }

  nh.spinOnce();
}
