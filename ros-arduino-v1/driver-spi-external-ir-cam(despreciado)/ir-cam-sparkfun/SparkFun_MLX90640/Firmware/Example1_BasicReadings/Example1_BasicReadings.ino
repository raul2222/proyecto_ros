#include <Wire.h>
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"

// For the ESP-WROVER_KIT, these are the default.


const byte MLX90640_address = 0x33; //Default 7-bit unshifted address of the MLX90640
#define TA_SHIFT 8 //Default shift for MLX90640 in open air
static float mlx90640To[768];
paramsMLX90640 mlx90640;
int xPos, yPos;                                // Abtastposition
int R_colour, G_colour, B_colour;              // RGB-Farbwert
int i, j;                                      // Zählvariable
float T_max, T_min;                            // maximale bzw. minimale gemessene Temperatur
float T_center;                                // Temperatur in der Bildschirmmitte
// ***************************************
// **************** SETUP ****************
// ***************************************
void setup()
   {
    Serial.begin(115200);    
    
    Wire.begin();
    Wire.setClock(400000); //Increase I2C clock speed to 400kHz
    while (!Serial); //Wait for user to open terminal
    
    Serial.println("MLX90640 IR Array Example");
    if (isConnected() == false)
       {
        Serial.println("MLX90640 not detected at default I2C address. Please check wiring. Freezing.");
        while (1);
       }
       
    Serial.println("MLX90640 online!");
    //Get device parameters - We only have to do this once
    int status;
    uint16_t eeMLX90640[832];
    
    status = MLX90640_DumpEE(MLX90640_address, eeMLX90640);
  
    if (status != 0)
       Serial.println("Failed to load system parameters");
    status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
  
    if (status != 0)
       {
        Serial.println("Parameter extraction failed");
        Serial.print(" status = ");
        Serial.println(status);
       }
    //Once params are extracted, we can release eeMLX90640 array
    MLX90640_I2CWrite(0x33, 0x800D, 6401);    // Schreibt den Wert 1901 (HEX) = 6401 (DEC) ins Register an die Stelle 0x800D, damit der Sensor ausgelesen werden kann!!!
                                              // =======================================================================================================================
    //MLX90640_SetRefreshRate(MLX90640_address, 0x00); //Set rate to 0.25Hz effective - Works
    //MLX90640_SetRefreshRate(MLX90640_address, 0x01); //Set rate to 0.5Hz effective - Works
    //MLX90640_SetRefreshRate(MLX90640_address, 0x02); //Set rate to 1Hz effective - Works
    //MLX90640_SetRefreshRate(MLX90640_address, 0x03); //Set rate to 2Hz effective - Works
    MLX90640_SetRefreshRate(MLX90640_address, 0x04); //Set rate to 4Hz effective - Works
    //MLX90640_SetRefreshRate(MLX90640_address, 0x05); //Set rate to 8Hz effective - Works at 800kHz
    //MLX90640_SetRefreshRate(MLX90640_address, 0x06); //Set rate to 16Hz effective - Works at 800kHz
    //MLX90640_SetRefreshRate(MLX90640_address, 0x07); //Set rate to 32Hz effective - fails
    
    
  delay(1000);
   

   } 
// *******************************************
// ************** HAUPTSCHLEIFE **************
// *******************************************
void loop()
   {
    for (byte x = 0 ; x < 2 ; x++) //Read both subpages
       {
        uint16_t mlx90640Frame[834];
        int status = MLX90640_GetFrameData(MLX90640_address, mlx90640Frame);
    
        if (status < 0)
           {
            Serial.print("GetFrame Error: ");
            Serial.println(status);
           }
        float vdd = MLX90640_GetVdd(mlx90640Frame, &mlx90640);
        float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);
        float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
        float emissivity = 0.95;
        MLX90640_CalculateTo(mlx90640Frame, &mlx90640, emissivity, tr, mlx90640To);
       }
       
    for (int x = 0 ; x < 768 ; x++)
    {
      if(x % 32 == 0) Serial.println();
      Serial.print(mlx90640To[x], 2);
      Serial.print(",");
    }
    Serial.println("");

 
    delay(1000);
   }
   
// =========================
// ===== Farbbestimmung ====
// =========================
void getColour(int j)
   {
    if (j >= 0 && j < 30)
       {
        R_colour = 0;
        G_colour = 0;
        B_colour = 20 + (120.0/30.0) * j;
       }
    
    if (j >= 30 && j < 60)
       {
        R_colour = (120.0 / 30) * (j - 30.0);
        G_colour = 0;
        B_colour = 140 - (60.0/30.0) * (j - 30.0);
       }
    if (j >= 60 && j < 90)
       {
        R_colour = 120 + (135.0/30.0) * (j - 60.0);
        G_colour = 0;
        B_colour = 80 - (70.0/30.0) * (j - 60.0);
       }
    if (j >= 90 && j < 120)
       {
        R_colour = 255;
        G_colour = 0 + (60.0/30.0) * (j - 90.0);
        B_colour = 10 - (10.0/30.0) * (j - 90.0);
       }
    if (j >= 120 && j < 150)
       {
        R_colour = 255;
        G_colour = 60 + (175.0/30.0) * (j - 120.0);
        B_colour = 0;
       }
    if (j >= 150 && j <= 180)
       {
        R_colour = 255;
        G_colour = 235 + (20.0/30.0) * (j - 150.0);
        B_colour = 0 + 255.0/30.0 * (j - 150.0);
       }
   }
   
   
//Returns true if the MLX90640 is detected on the I2C bus
boolean isConnected()
   {
    Wire.beginTransmission((uint8_t)MLX90640_address);
  
    if (Wire.endTransmission() != 0)
       return (false); //Sensor did not ACK
    
    return (true);
   }
