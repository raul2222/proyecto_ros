
const byte numChars = 3000;
<<<<<<< HEAD:ros-arduino/driver-serial-ir-cam/opencr-p/opencr-p.ino
char receivedChars[numChars];

boolean newData = false;
int num = 0;
boolean recvInProgress = false;
=======
String receivedChars="";
boolean recvInProgress = false;
boolean newData = false;
int num = 0;
>>>>>>> develop:ros-arduino-v1/driver-serial-ir-cam/2-opencr-esp32-clon/2-opencr-esp32-clon.ino

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("<Arduino is ready>");
    Serial2.begin(115200);delay(500);
}

void loop() {
<<<<<<< HEAD:ros-arduino/driver-serial-ir-cam/opencr-p/opencr-p.ino
    delay(7000);
    Serial.println("esto es loop");
    getImageFromOpenCR();
    delay(50);  
    
  
}

void getImageFromOpenCR(){ 
    num++;
    
    if(recvInProgress == false) {
      Serial.println("peticion de frame ");
      Serial.println(num);
      Serial2.write("<IR>");
      delay(10);
      recvWithStartEndMarkers();
      showNewData();
    }
}

void recvWithStartEndMarkers() {
    int enter = millis();
=======
    if(recvInProgress == false){
        delay(500);
        //Serial.println("esto es loop");
        receivedChars="";
        getImage();
        delay(1);  
    }
    recvWithStartEndMarkers();
    showNewData();
    delay(1);
}

void getImage(){ 
    num++;
    Serial.println("peticion de frame ");
    Serial.println(num);
    Serial2.write("<IR>");
    delay(1);
}

void recvWithStartEndMarkers() {
    
>>>>>>> develop:ros-arduino-v1/driver-serial-ir-cam/2-opencr-esp32-clon/2-opencr-esp32-clon.ino
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
    boolean cancel = false;

    int init = millis();
 
    while (Serial2.available() > 0 && newData == false && cancel == false) {
        rc = Serial2.read();
        //if
        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars = receivedChars + rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                //receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        } else if (rc == startMarker) {
            recvInProgress = true;
        } //fin if
        
        if(millis() > init+100){ //whatchdog software
          cancel = true;
          recvInProgress == false;
        }
<<<<<<< HEAD:ros-arduino/driver-serial-ir-cam/opencr-p/opencr-p.ino
        // cancelo la accion si tarda mas de 1 segundo
        //if(millis() > (enter + 1000)){
        //  newData = true;
        //}
    }
=======
        
    } // fin while()
    
>>>>>>> develop:ros-arduino-v1/driver-serial-ir-cam/2-opencr-esp32-clon/2-opencr-esp32-clon.ino
}

void showNewData() {
    if (newData == true) {
<<<<<<< HEAD:ros-arduino/driver-serial-ir-cam/opencr-p/opencr-p.ino
          Serial.println("This just in ... ");
          Serial.println(receivedChars);
          newData = false;
=======
        Serial.println("This just in ... ");
        Serial.println(receivedChars);
        newData = false;
        receivedChars = "";
>>>>>>> develop:ros-arduino-v1/driver-serial-ir-cam/2-opencr-esp32-clon/2-opencr-esp32-clon.ino
    }
}
