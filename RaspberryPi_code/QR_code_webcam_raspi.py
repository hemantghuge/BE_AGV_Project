#use https://www.the-qrcode-generator.com/ website to generate QR code.
"""
Project Name: Smart and Efficient Techniques for Automated Guided Vehicles
Author List: Unnati R Kulkarni
Filename: QR_webcam.py
Functions: video_capture.read(), pyzbar.decode(frame), video_capture.release(), cv2.destroyAllWindows()
Libraries: pyzbar, numpy
"""

import cv2 as cv
import numpy as np
import pyzbar.pyzbar as pyzbar
import time

width = 640
height = 480
fps = 30
percent = 100

video_capture = cv.VideoCapture(0) #open the default camera that is webcam
video_capture.set(3,width)
video_capture.set(4,height)
video_capture.set(5,fps)

fourcc = cv.VideoWriter_fourcc(*'XVID')
#out = cv.VideoWriter('videoAfterProcessing.avi',fourcc, fps, (width,height))
#out1 = cv.VideoWriter('videoThreshold.avi', fourcc, fps, (width,height))
video_capture.set(cv.CAP_PROP_FOURCC,cv.VideoWriter_fourcc(*'MJPG'))

#video_capture = cv2.Videovideo_captureture(0) #open the default camera that is webcam
font = cv.FONT_HERSHEY_PLAIN #font to be displayed on screen

#continuous loop 
while True:
   
#Function: video_capture.read
#Input: None
#Logic: Returns True if frame is read correctly.
    
    _, frame = video_capture.read()


#Function: pyzbar.decode
#Input: frame
#Output: decoded object (QR code)
#Logic: Return symbols from the frame
    
    decodedObjects = pyzbar.decode(frame)
    for obj in decodedObjects:
        
        cv.putText(frame, str(obj.data), (50, 50), font, 2,
                    (255, 0, 0), 3)  #inserting text on the frame

    cv.imshow("Frame", frame)  #display image with QR text
    
    if cv.waitKey(1) & 0XFF == ord('q'): 
        break

"""

Function: video_capture.release
Input: None
Output: None
Logic: release camera object

"""

video_capture.release()

"""

Function: destroyAllWindows
Input: None
Output: None
Logic: closing all the windows

"""

cv.destroyAllWindows()

