
const byte numChars = 1000;
char receivedChars[numChars];

boolean newData = false;
int num = 0;

void setup() {
    Serial.begin(19200);
    delay(5000);
    Serial.println("<Arduino is ready>");
    Serial2.begin(57600);delay(500);
}

void loop() {
    delay(7000);
    Serial.println("esto es loop");
    getImage();
    delay(50);  
    recvWithStartEndMarkers();
    showNewData();
}

void getImage(){ 
    num++;
    Serial.println("peticion de frame ");
    Serial.print(num);
    Serial2.write("<IR>");
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
        Serial.println("This just in ... ");
        Serial.println(receivedChars);
        newData = false;
    }
}
