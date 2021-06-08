
const byte numChars = 3000;
char receivedChars[numChars];

boolean newData = false;
int num = 0;
boolean recvInProgress = false;

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("<Arduino is ready>");
    Serial2.begin(115200);delay(500);
}

void loop() {
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
        // cancelo la accion si tarda mas de 1 segundo
        //if(millis() > (enter + 1000)){
        //  newData = true;
        //}
    }
}

void showNewData() {
    if (newData == true) {
          Serial.println("This just in ... ");
          Serial.println(receivedChars);
          newData = false;
    }
}
