
#include <Wire.h>

void setup()
{
  Serial.begin(9600); 
  delay(3000);
  Serial.print("Inicio del master i2c en esp 32 {1,-2,5,1,8,2,4,10}");
  Wire.begin(); // join i2c bus (address optional for master)
}

byte x = 0;

void loop()
{
  //el esp como master envia la imagen 
  Wire.beginTransmission(4); // transmit to device #4
  Wire.write("{ 1,2,3,4,5,6 }");        // sends five bytes
  Wire.endTransmission();    // stop transmitting

  x++;
  delay(5000);
}
