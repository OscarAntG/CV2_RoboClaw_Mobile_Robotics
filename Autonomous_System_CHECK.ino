#include <RoboClaw.h>
#include <SoftwareSerial.h>
// Define RoboClaw address
#define ROBOCLAW_ADDRESS 0x80  // Default address

// Initialize RoboClaw object
SoftwareSerial ROBOSERIAL(10, 11);
RoboClaw roboclaw(&ROBOSERIAL, 10000);

void setup() {
    // Start communication
    Serial.begin(115200);  // Communication with Raspberry Pi
    roboclaw.begin(38400); // Initialize RoboClaw
}

void loop() {
    // Check if data is available from Raspberry Pi
    if (Serial.available()){
      String data = Serial.readStringUntil('\n');
      int cx = 0;
      sscanf(data.c_str(), "%d", &cx);
      findPosistionfromCenter(cx);
    }
}

void findPosistionfromCenter(int cx) {
  if (cx >= 0) {
    roboclaw.ForwardM1(ROBOCLAW_ADDRESS, constrain(cx, 0, 127));
  } else if (cx < 0) {
    roboclaw.ForwardM2(ROBOCLAW_ADDRESS, constrain(abs(cx), 0, 127));
  } else {
    roboclaw.ForwardM1(ROBOCLAW_ADDRESS, 0);
    roboclaw.ForwardM2(ROBOCLAW_ADDRESS, 0);
  }
}
