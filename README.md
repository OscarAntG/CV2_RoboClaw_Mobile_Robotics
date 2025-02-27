This code is to make a semi-autonomous robot move with a detected color line via OpenCV
This code takes a camera frame's width and creates a center. If the target is to the left or right of that center.

Then the robot moves towards the target. amazing stuff.
The Arduino Mega has multiple serial ports so we took advantage of that by enabling one serial port to talk to a roboclaw motor driver, and enable another serial port to receive commands from computer when running the center line python file, sending the direction needed to the arduino.

The manual failsafe is attached to the arduino. The arduino takes the PWM signal from the RC remote and translates that to the RoboClaw. We flick a switch on the remote then it overrides the autonomous functions of the robot. Then the RC channels that we toggle are receieved by the arduino and translated to the roboclaw. This would be usally unneccesary but this was the best way to do a failsafe that is hardware focused  rather than a software focus.
