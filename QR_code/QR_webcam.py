#use https://www.the-qrcode-generator.com/ website to generate QR code.
"""
Project Name: Smart and Efficient Techniques for Automated Guided Vehicles
Author List: Unnati
Filename: qr_web_01.py
Functions: cap.read(), pyzbar.decode(frame), cap.release(), cv2.destroyAllWindows()
Libraries: pyzbar, numpy
"""

import cv2
import numpy as np
import pyzbar.pyzbar as pyzbar

cap = cv2.VideoCapture(0) #open the default camera that is webcam
font = cv2.FONT_HERSHEY_PLAIN #font to be displayed on screen

#continuous loop 
while True:
   
#Function: cap.read
#Input: None
#Logic: Returns True if frame is read correctly.
    
    _, frame = cap.read()


#Function: pyzbar.decode
#Input: frame
#Output: decoded object (QR code)
#Logic: Return symbols from the frame
    
    decodedObjects = pyzbar.decode(frame)
    for obj in decodedObjects:
        
        cv2.putText(frame, str(obj.data), (50, 50), font, 2,
                    (255, 0, 0), 3)  #inserting text on the frame

    cv2.imshow("Frame", frame)  #display image with QR text
    
    if cv2.waitKey(1) & 0XFF == ord('q'): 
        break

"""

Function: cap.release
Input: None
Output: None
Logic: release camera object

"""

cap.release()

"""

Function: destroyAllWindows
Input: None
Output: None
Logic: closing all the windows

"""

cv2.destroyAllWindows()
