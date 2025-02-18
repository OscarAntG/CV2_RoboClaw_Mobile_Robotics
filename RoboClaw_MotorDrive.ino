#include <RoboClaw.h>
#include <SoftwareSerial.h>

 // Change to match your Arduino's hardware serial port for RoboClaw
#define PI_SERIAL Serial  // Raspberry Pi communication via USB
#define ROBOCLAW_ADDRESS 0x80 // Default address

// Initialize RoboClaw object
SoftwareSerial serial(10, 11);
RoboClaw roboclaw(&serial, 10000);

void setup() {
  // put your setup code here, to run once:
  PI_SERIAL.begin(115200);  // Communication with Raspberry Pi
  // Initialize RoboClaw
    roboclaw.begin(38400);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (PI_SERIAL.available()){
    String data = PI_SERIAL.readStringUntil('\n');
    int cx = 0;
    sscanf(data.c_str(), "%d", &cx);
    findPosistionfromCenter(cx);
  }
}

void findPosistionfromCenter(int cx) {
  if (cx >= 0) {
    roboclaw.ForwardM1(ROBOCLAW_ADDRESS, constrain(cx, 0, 127));
  } else if (cx < 0) {
    roboclaw.ForwardM2(ROBOCLAW_ADDRESS, constrain(cx, 0, 127));
  } else {
    roboclaw.ForwardM1(ROBOCLAW_ADDRESS, 0);
    roboclaw.ForwardM2(ROBOCLAW_ADDRESS, 0);
  }
}
