// This is using the Arduino Mega for its multiple Serial ports
// This is vital for the functionalilty of this autonomous system
// Author John Marcial and Oscar Gonzalez
#include <RoboClaw.h>

#define defaultSpeedMotor1 96
#define defaultSpeedMotor2 223
#define stopMotor1 64
#define stopMotor2 192

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
      int cx, error = 0;
      sscanf(data.c_str(), "%d,%d", &cx, &error);
      findPosistionfromCenter(cx, error);
    }
}

void findPosistionfromCenter(int cx, int error) {
  if (cx < -106) {
    Serial1.write(defaultSpeedMotor1 + error)));  // Foward Motor 1 by a percent amount 
    Serial1.write(stopMotor2 - error);  // Reverses Motor 2 by a percent amount Stop is 192 if 100% then hits reverse by taking 64 to make 128
  } else if (cx > 106) {
    Serial1.write(defaultSpeedMotor2 - error);  // 128	Channel 2 full forward
    Serial1.write(stopMotor1 - error);  // 64	Channel 1 stop
  } else if ( cx != 0 && cx >= -106 && cx <= 106){
    Serial1.write(defaultSpeedMotor1);  // 127	Channel 1 full forward
    Serial1.write(defaultSpeedMotor2);  // 128	Channel 2 full forward
  } else{
    terminateInactive();
  }
}

void terminateInactive(){
  Serial1.write(0); // 0	Shuts down channels 1 and 2
}
