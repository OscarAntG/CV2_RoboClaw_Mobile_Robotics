# Importing Libraries
import serial.tools.list_ports
import numpy as np
import cv2

# Setup of the COM port communcation (Windows Only)
ports = serial.tools.list_ports.grep('COM')
# Setup of the COM port communcation (Linux Only)
#ports = serial.tools.list_ports.grep('tty')
serialInst = serial.Serial()

# Way to list all ports available
portsList = []

for onePort in ports:
    portsList.append(str(onePort))
    print(str(onePort))

# Choose the port you want to connect to
val = input('Select Port: COM')  # (Windows Only)
# val - input('Select Port: /dev/ttyACM')  # (Linux Only)

for x in range(0, len(portsList)):
    if portsList[x].startswith('COM' + str(val)):  # Windows
    #if portsList[x].startswith('/dev/ttyACM' + str(val)):  # Linux
        portVar = 'COM' + str(val)
        #portVar = '/dev/ttyACM' + str(val)
        print(portVar)

# Change the baudrate and then it opens the channel
serialInst.baudrate = 115200
serialInst.port = portVar
serialInst.open()

# Darker end of the spectrum
Dark_red_LowerLimit = np.array([170, 150, 150], dtype=np.uint8)  # Lower limit for red hue  WORKS
Dark_red_UpperLimit = np.array([180, 255, 255], dtype=np.uint8)  # Upper limit for red hue WORKS

# Lighter end of the red spectrum
Red_LowerLimit = np.array([0, 150, 150], dtype=np.uint8)
Red_upperLimit = np.array([10, 255, 255], dtype=np.uint8)

# Green color detection limits
green_lower = np.array([50, 50, 50], dtype=np.uint8)
green_upper = np.array([90, 255, 255], dtype=np.uint8)

# Yellow color detection limits
yellow_lower = np.array([20, 50, 50], dtype=np.uint8)
yellow_higher = np.array([50, 255, 255], dtype=np.uint8)

# Blue color detection limits
blue_lower = np.array([100, 100, 100], dtype=np.uint8)
blue_higher = np.array([130, 255, 255], dtype=np.uint8)

# How to change between mask easily (0=DR...2=green...4=blue)
lower_color = [Dark_red_LowerLimit, Red_LowerLimit, green_lower, yellow_lower, blue_lower]
higher_color = [Dark_red_UpperLimit, Red_upperLimit, green_upper, yellow_higher, blue_higher]

# filter resolution
kernel = np.ones((25, 25), np.uint8)

# Initializing Video Capture #
cap = cv2.VideoCapture(0)

# Use this to get the max width frames to know when to move left and right and by how much.
default_width = cap.get(cv2.CAP_PROP_FRAME_WIDTH)
# print("Default frame width:", default_width)  #CALIBRATION PURPOSES ONLY

# Accesses camera and finds the targeted color and displays it
while True:
    ret, frame = cap.read()
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, lower_color[2], higher_color[2])
    # Activate mask 2 only when you are detecting red
    # mask2 = cv2.inRange(hsv, lower_color[1], higher_color[1])
    # mask = mask & mask2
    mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
    mask_contours, hierarchy = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    results = frame.copy()
    results = cv2.drawContours(results, mask_contours, -1, (0, 0, 255), 3)

    # The natural state of command should be 0 in case there is no color detected
    command = f'0\n'

    # crates a box around the targeted color
    if len(mask_contours) != 0:
        for mask_contours in mask_contours:
            if cv2.contourArea(mask_contours) > 500:
                # This finds the center of the color
                M = cv2.moments(mask_contours)
                cX = int(M["m10"] / M["m00"])
                cY = int(M["m01"] / M["m00"])
                cv2.circle(results, (cX, cY), 7, (0, 0, 0), -1)
                # Find the center and the distance from the center
                movement_variable = cX - (default_width/2)
                # Sends the distance from the center as a command to the arduino
                command = f'{movement_variable}\n'
                print(f'Move by: {movement_variable}')  #DEGUGGING PURPOSES ONLY


                # On the arduino end make it receive these number fi it's a negative take the absolute and move only the
                # left motors if positive just move the right motors

    # Send the command
    serialInst.write(command.encode('utf-8'))

# This shows the photo with the box, the mask, and what the computer will see before putting a box this is for debugging
# purposes
#     cv2.imshow('Frame', frame)
#     cv2.imshow('HSV', hsv)
#     cv2.imshow('Mask', mask)
    cv2.imshow('Results', results)  #You need this to close actually

# Waits for the user to hit the q button to close program #
    if cv2.waitKey(1) == ord('q'):
        break

# Allows to release the picture to free used system resources #
cap.release()
cv2.destroyAllWindows()
