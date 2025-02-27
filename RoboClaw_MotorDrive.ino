// THIS ENTIRE CODE NEEDS TO BE TESTED FOR LOGIC CONTROL AND SAFTEY IMPLEMENTATIONS
// This is using the Arduino Mega for its multiple Serial ports
// This is vital for the functionalilty of this autonomous system
// Author John Marcial and Oscar Gonzalez
#include <RoboClaw.h>

// Channles to record from the TX hand remote (Use PMW Pins)
#define CH1 6
#define CH2 5
#define CH8 3

// Initialize RoboClaw object
RoboClaw roboclaw(&Serial1, 10000);

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
  Serial.begin(115200);
  Serial1.begin(38400); 
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
  }
  else if (Serial.available() && ch8Value != 1){
    String data = Serial.readStringUntil('\n');
    int cx = 0;
    sscanf(data.c_str(), "%d", &cx);
    findPosistionfromCenter(cx);
  }
  else{
    terminateInactive();
  }
}

// NEEDS TESTING 
// We can add backwards and forwards driving if we get this right
void RX_OverRide(int channel1, int channel2){
  if ((channel1 > 0) && (channel2 > 0)){
    Serial1.write(127);  // 127	Channel 1 full forward
    Serial1.write(128);  // 128	Channel 2 full forward
  } else if ((channel1 > 0) && (channel2 !> 0)){
    Serial1.write(127);  // 127	Channel 1 full forward
    Serial1.write(192);  // 192	Channel 2 stop
  } else if ((channel2 > 0) && (channel1 !> 0)){
    Serial1.write(128);  // 128	Channel 2 full forward
    Serial1.write(64);  // 64	Channel 1 stop
  } else{
    ternminateInactive();
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
