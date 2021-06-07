#include "SPI.h"

void setup() {
   digitalWrite(SS, HIGH); // disable Slave Select
   SPI.begin();
   SPI.setClockDivider(SPI_CLOCK_DIV4);//divide the clock by 4
}
 
void loop() {
   char c;
   digitalWrite(SS, LOW); // enable Slave Select
   for (const char * p = "Hello Mega\r" ; c = *p; p++) 
   {
      SPI.transfer(c);
   }
   digitalWrite(SS, HIGH); // disable Slave Select
   delay(2000);
}
