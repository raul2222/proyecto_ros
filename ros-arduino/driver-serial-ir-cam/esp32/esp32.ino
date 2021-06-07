#include <Wire.h>

#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"

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

void setup() {
    Serial.begin(57600);
    Serial2.begin(57600);
    delay(5000);
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

    MLX90640_SetRefreshRate(MLX90640_address, 0x02); //Set rate to 2Hz
  //MLX90640_SetRefreshRate(MLX90640_address, 0x03); //Set rate to 4Hz
  //MLX90640_SetRefreshRate(MLX90640_address, 0x07); //Set rate to 64Hz
    
}

void loop() {
    recvWithStartEndMarkers();
    showNewData();
    //Serial.println("loop");

    if(SendFrame == true){
        Serial.println("loop2");
        getImage();
        SendFrame == false;
        newData = false;
    }
    
    delay(10);
    
    //Serial2.println("<{1,2}>");
    
}


int getImage(){
  
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
  for (int x = 0 ; x < 768 ; x++)
  {
    if(x % 32 == 0) Serial.println();
    int value = abs(mlx90640To[x]);
    imagen_salida[x]=value;
    if (value < 0 || value > 200){
      validFrame = false;
    }
    Serial.print(value);
    Serial.print(",");
  }

  if (validFrame == true){
    Serial2.print("<{");
    for (int x = 0 ; x < 768 ; x++){
      Serial2.print(imagen_salida[x]);
      Serial2.print(",");
    }
    Serial2.print("}>");
  }

  
  Serial.println("");   
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
        //Serial.print("This just in ... ");
        Serial.println(receivedChars);
        if (StrContains(receivedChars, "IR") > 0){
          Serial.println("entra");
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
