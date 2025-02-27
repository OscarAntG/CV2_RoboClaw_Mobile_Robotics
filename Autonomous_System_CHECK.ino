// This is using the Arduino Mega for its multiple Serial ports
// This is vital for the functionalilty of this autonomous system
// Author John Marcial and Oscar Gonzalez
#include <RoboClaw.h>

// Initialize RoboClaw object
RoboClaw roboclaw(&Serial1, 10000);

void setup() {
    // Start communication
    Serial.begin(115200);  // Communication with computer
    Serial1.begin(38400);  
    roboclaw.begin(38400); // Initialize RoboClaw
}

void loop() {
    // Check if data is available from computer
    if (Serial.available()){
      String data = Serial.readStringUntil('\n');
      int cx = 0;
      sscanf(data.c_str(), "%d", &cx);
      findPosistionfromCenter(cx);
    }
}

void findPosistionfromCenter(int cx) {
  if (cx > 48) {
    Serial1.write(127);  // 127	Channel 1 full forward
    Serial1.write(192);  // 192	Channel 2 stop
  } else if (cx < -48) {
    Serial1.write(128);  // 128	Channel 2 full forward
    Serial1.write(64);  // 64	Channel 1 stop
  } else if ( cx != 0 && cx >= -48 && cx <= 48){
    Serial1.write(127);  // 127	Channel 1 full forward
    Serial1.write(128);  // 128	Channel 2 full forward
  } else{
    terminateInactive();
  }
}

void terminateInactive(){
  Serial1.write(0); // 0	Shuts down channels 1 and 2
}
