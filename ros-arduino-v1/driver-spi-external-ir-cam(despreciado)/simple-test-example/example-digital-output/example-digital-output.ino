#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Byte.h>

# define LED_3 7
# define LED_2 6
# define LED_1 5
# define LED_0 4

int led_pin_user[4] = {LED_0, LED_1, LED_2, LED_3};

ros::NodeHandle  nh;

void messageCb( const std_msgs::Byte& led_msg) {
  int i;

  for (i=0; i<4; i++)
  {
    if (led_msg.data & (1<<i))
    {
      digitalWrite(led_pin_user[i], HIGH);
    }
    else
    {
      digitalWrite(led_pin_user[i], LOW);
    }
  }
}

ros::Subscriber<std_msgs::Byte> sub("led_out", messageCb );


void setup() {
  pinMode(led_pin_user[0], OUTPUT);
  pinMode(led_pin_user[1], OUTPUT);
  pinMode(led_pin_user[2], OUTPUT);
  pinMode(led_pin_user[3], OUTPUT);

  nh.initNode();
  nh.subscribe(sub);
}

void loop() {
  nh.spinOnce();
}
