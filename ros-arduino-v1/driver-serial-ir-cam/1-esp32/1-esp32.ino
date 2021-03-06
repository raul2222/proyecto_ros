#include <Wire.h>
#include <esp_task_wdt.h>
//7 seconds WDT
<<<<<<< HEAD:ros-arduino/driver-serial-ir-cam/esp32/esp32.ino
#define WDT_TIMEOUT 7
=======
#define WDT_TIMEOUT 4
>>>>>>> develop:ros-arduino-v1/driver-serial-ir-cam/1-esp32/1-esp32.ino
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"
#define DEBUG 1 

const byte numChars = 32;
char receivedChars[numChars];
/////
boolean newData = false;
boolean SendFrame = false;
uint8_t imagen_salida[768];
const byte MLX90640_address = 0x33; //Default 7-bit unshifted address of the MLX90640

#define TA_SHIFT 8 //Default shift for MLX90640 in open air
float mlx90640To[768];
paramsMLX90640 mlx90640;

String Frame = "";
int i = 0;
int last = millis();
int mycount = 0;

void setup() {
<<<<<<< HEAD:ros-arduino/driver-serial-ir-cam/esp32/esp32.ino
    Serial.begin(115200);
    Serial2.begin(115200);
    delay(1000);
=======
    Serial.begin(9600);
    Serial2.begin(57600);
    delay(1500);
>>>>>>> develop:ros-arduino-v1/driver-serial-ir-cam/1-esp32/1-esp32.ino
    Serial.println("Configuring WDT...");
    esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
    esp_task_wdt_add(NULL); //add current thread to WDT watch
    Serial.println("<Arduino is ready>");
    Wire.begin();
    Wire.setClock(400000); //Increase I2C clock speed to 400kHz

    if (isConnected() == false)
    {
      Serial.println("MLX90640 not detected at default I2C address. Please check wiring. Freezing.");
      //while (1);
    }

    //Get device parameters - We only have to do this once
    int status;
    uint16_t eeMLX90640[832];
    status = MLX90640_DumpEE(MLX90640_address, eeMLX90640);
    if (status != 0)
      Serial.println("Failed to load system parameters");

    status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
    if (status != 0)
      Serial.println("Parameter extraction failed");

    //Once params are extracted, we can release eeMLX90640 array
    MLX90640_I2CWrite(0x33, 0x800D, 6401);// Schreibt den Wert 1901 (HEX) = 6401 (DEC) ins Register an die Stelle 0x800D, damit der Sensor ausgelesen werden kann!!!
<<<<<<< HEAD:ros-arduino/driver-serial-ir-cam/esp32/esp32.ino
    MLX90640_SetRefreshRate(MLX90640_address, 0x04); //Set rate to 2Hz
  //MLX90640_SetRefreshRate(MLX90640_address, 0x03); //Set rate to 4Hz
  //MLX90640_SetRefreshRate(MLX90640_address, 0x07); //Set rate to 64Hz
=======
    //MLX90640_SetRefreshRate(MLX90640_address, 0x04); //Set rate to 2Hz
    MLX90640_SetRefreshRate(MLX90640_address, 0x03); //Set rate to 4Hz
    //MLX90640_SetRefreshRate(MLX90640_address, 0x07); //Set rate to 64Hz
>>>>>>> develop:ros-arduino-v1/driver-serial-ir-cam/1-esp32/1-esp32.ino

    esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
    esp_task_wdt_add(NULL); //add current thread to WDT watch
    delay(100);
    Serial.println("fin setup");
}

void loop() {
<<<<<<< HEAD:ros-arduino/driver-serial-ir-cam/esp32/esp32.ino
    /*
    if (millis() - last >= 2000 && i < 5) {
        Serial.println("Resetting WDT...");
        esp_task_wdt_reset();
        last = millis();
        i++;
        if (i == 5) {
          Serial.println("Stopping WDT reset. CPU should reboot in 3s");
        }
    }*/
    //whtchdog esp32
=======
    //whatchdog esp32
>>>>>>> develop:ros-arduino-v1/driver-serial-ir-cam/1-esp32/1-esp32.ino
    esp_task_wdt_reset();

    recvWithStartEndMarkers();
    showNewData();

    if(SendFrame == true){
        //Serial.println("loop2");
        getImage();
        SendFrame == false;
        newData = false;
        int last = millis();
    }
    
<<<<<<< HEAD:ros-arduino/driver-serial-ir-cam/esp32/esp32.ino
    delay(500);
    mycount++;
    if (mycount > 20 ){
      getImage();  

      mycount = 0;
    }

    


=======
    delay(1);
    // si no recibo una peticion en 15 segundos provoco un reset
    if(millis() > (last+15000)){
      delay (4100);
    }
>>>>>>> develop:ros-arduino-v1/driver-serial-ir-cam/1-esp32/1-esp32.ino
#ifdef DEBUG
    //Serial.println("loop1");
#endif

}


int getImage(){
<<<<<<< HEAD:ros-arduino/driver-serial-ir-cam/esp32/esp32.ino
  Serial.println("Image ");
=======
  i++;
  Serial.print("Image "); 
  Serial.println(i);
  char buff[110];
>>>>>>> develop:ros-arduino-v1/driver-serial-ir-cam/1-esp32/1-esp32.ino
  long startTime = millis();
  int status = 0;
  for (byte x = 0 ; x < 2 ; x++)
  {
    uint16_t mlx90640Frame[834];
    status = MLX90640_GetFrameData(MLX90640_address, mlx90640Frame);

    float vdd = MLX90640_GetVdd(mlx90640Frame, &mlx90640);
    float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);

    float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
    float emissivity = 0.95;

    MLX90640_CalculateTo(mlx90640Frame, &mlx90640, emissivity, tr, mlx90640To);
  }
  long stopTime = millis();
  boolean validFrame = true;
  Frame = "<{";

  for (int x = 0 ; x < 768 ; x++)
  {
<<<<<<< HEAD:ros-arduino/driver-serial-ir-cam/esp32/esp32.ino
    if(x % 32 == 0) 
    if(isnan(mlx90640To[x])){
      validFrame = false;
    }
    //Serial.print(mlx90640To[x]);
    Frame = Frame + mlx90640To[x] + ",";
    //Serial.print(","); 
  }
  if(validFrame == false){
    Serial.println("invalid");
    delay (7000);
  }


  
  if(SendFrame == true) {
    Frame.remove(769);
    Frame = Frame + "}>";
    Serial.println(Frame);
    Serial2.println(Frame);
=======
    //Serial.print(mlx90640To[x]);
    Frame = Frame + mlx90640To[x] + ",";
    //Serial.print(","); 
  }
  Frame.toCharArray(buff,100);
  //Serial.println(buff);
  if(StrContains(buff, "nan") > 0 ){
    validFrame = false;
  }
  if(validFrame == false){
    Serial.println("invalid");
    delay (4100);
  }

  if(SendFrame == true) {
    Frame.remove(769);
    Frame = Frame + "}>";

    Serial2.write(Frame.c_str());
    delay(1);
    Serial.print(Frame.substring(0,10));
    delay(1);
    //Serial.println(Frame.substring(386,772));

>>>>>>> develop:ros-arduino-v1/driver-serial-ir-cam/1-esp32/1-esp32.ino
  }

  SendFrame = false;
  return status;
}


//Returns true if the MLX90640 is detected on the I2C bus
boolean isConnected()
{
    Wire.beginTransmission((uint8_t)MLX90640_address);
    if (Wire.endTransmission() != 0)
      return (false); //Sensor did not ACK
    return (true);
}


void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (Serial2.available() > 0 && newData == false) {
        rc = Serial2.read();
        Serial.print(rc);
        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        Serial.print("This just in ... ");
        Serial.println(receivedChars);
        if (StrContains(receivedChars, "IR") > 0){
          //Serial.println("entra");
          SendFrame = true;
        } else {
          newData = false;
          SendFrame = false;
        }
    }
}

// searches for the string sfind in the string str
// returns 1 if string found
// returns 0 if string not found
char StrContains(char *str, char *sfind)
{
    char found = 0;
    char index = 0;
    char len;

    len = strlen(str);
    
    if (strlen(sfind) > len) {
        return 0;
    }
    while (index < len) {
        if (str[index] == sfind[found]) {
            found++;
            if (strlen(sfind) == found) {
                return 1;
            }
        }
        else {
            found = 0;
        }
        index++;
    }

    return 0;
}
