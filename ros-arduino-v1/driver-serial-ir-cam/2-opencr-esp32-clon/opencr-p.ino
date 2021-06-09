
const byte numChars = 3000;
String receivedChars="";
boolean recvInProgress = false;
boolean newData = false;
int num = 0;

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("<Arduino is ready>");
    Serial2.begin(115200);delay(500);
}

void loop() {
  if(recvInProgress == false){
      delay(7000);
      Serial.println("esto es loop");
      getImage();
      delay(1);  
  }
    recvWithStartEndMarkers();
    showNewData();
    delay(1);
}

void getImage(){ 
    if(recvInProgress == false){
      receivedChars="";
      num++;
      Serial.println("peticion de frame ");
      Serial.println(num);
      Serial2.write("<IR>");
      delay(1);
    }
    recvWithStartEndMarkers();
    showNewData();
    if(recvInProgress==true){
      Serial.println("rev");
    }
    
    delay(1);
}

void recvWithStartEndMarkers() {
    
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (Serial2.available() > 0 && newData == false) {
        rc = Serial2.read();
        //Serial.print(rc);
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
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        Serial.println("This just in ... ");
        Serial.println(receivedChars);
        newData = false;
    }
}
