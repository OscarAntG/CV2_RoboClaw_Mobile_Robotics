#include <RoboClaw.h>
#include <SoftwareSerial.h>

// Channles to record from the TX hand remote
#define CH1 6
#define CH2 5
#define CH8 3

 // Change to match your Arduino's hardware serial port for RoboClaw
#define ROBOCLAW_ADDRESS 0x80 // Default address

// Initialize RoboClaw object
SoftwareSerial serial(10, 11);
RoboClaw roboclaw(&serial, 10000);

// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Red the channel and return a boolean value
bool redSwitch(byte channelInput, bool defaultValue){
  int intDefaultValue = (defaultValue)? 100: 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

void setup() {
  // put your setup code here, to run once:
  // Initialize RoboClaw
  roboclaw.begin(38400);
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH8, INPUT);
    
}

int ch1Value, ch2Value;
bool ch8Value;

void loop() {
  // put your main code here, to run repeatedly:
  ch1Value = readChannel(CH1, -100, 100, 0);
  ch2Value = readChannel(CH2, -100, 100, 0);
  ch8Value = redSwitch(CH8, false);

  if (ch8Value == 1){
    RX_OverRide(ch1Value, ch2Value);
    if (PI_SERIAL.available()){
      String data = PI_SERIAL.readStringUntil('\n');
      int cx = 0;
      sscanf(data.c_str(), "%d", &cx);
      findPosistionfromCenter(cx);
    }
  }
}

void RX_OverRide(int channel1, int channel2){
  roboclaw.ForwardM1(ROBOCLAW_ADDRESS, constrain(channel1, 0, 127));
  roboclaw.ForwardM2(ROBOCLAW_ADDRESS, constrain(channel2, 0, 127));
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
