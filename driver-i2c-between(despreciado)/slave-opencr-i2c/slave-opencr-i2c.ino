//#include <Wire.h>
#include <WireSlave.h>

#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_SLAVE_ADDR 4


void setup()
{

  Serial.begin(9600);           // start serial for output
  delay(3000);
  Serial.print("inicio de i2c en open cr");

  
  //Wire.begin(4);                // join i2c bus with address #4
  //Wire.onReceive(receiveEvent); // register event

  bool success = WireSlave.begin(SDA_PIN, SCL_PIN, I2C_SLAVE_ADDR);
    if (!success) {
        Serial.println("I2C slave init failed");
        while(1) delay(100);
    }

  WireSlave.onReceive(receiveEvent);
  

}

void loop()
{
  delay(1000);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  Serial.println("HowMany: ");  
  Serial.println(howMany);
  while(1 < WireSlave.available()) // loop through all but the last
  {
    char c = WireSlave.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = WireSlave.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}
