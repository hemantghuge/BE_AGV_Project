# Python program to illustrate HoughLine 
# method for line detection 
import cv2 as cv
import numpy as np
import serial
import RPi.GPIO as GPIO
import time,sys

SERIAL_PORT = "/dev/ttyS0"
ser = serial.Serial(SERIAL_PORT, baudrate = 9600)

width = 640
height = 480
fps = 30
percent = 100

video_capture = cv.VideoCapture(0)
video_capture.set(3,width)
video_capture.set(4,height)
video_capture.set(5,fps)

fourcc = cv.VideoWriter_fourcc(*'XVID')
#out = cv.VideoWriter('videoAfterProcessing.avi',fourcc, fps, (width,height))
#out1 = cv.VideoWriter('videoThreshold.avi', fourcc, fps, (width,height))
video_capture.set(cv.CAP_PROP_FOURCC,cv.VideoWriter_fourcc(*'MJPG'))

font = cv.FONT_HERSHEY_PLAIN #font to be displayed on screen


def rescale_frame(frame, percent):
    width = int(frame.shape[1]*percent/100)
    height = int(frame.shape[0]*percent/100)
    dim = (width, height)
    return cv.resize(frame, dim, interpolation = cv.INTER_AREA)

def show_graphics():
    cv.putText(crop_img, 'Angle ='+ str(angle_int),(400,50), cv.FONT_HERSHEY_SIMPLEX, 1, (20, 220, 250), 3)
    cv.imshow('crop_img',crop_img)
    cv.imshow('edges', edges)
    return 0

def data_transmission(angle):
    ser.write(angle.encode())
    print('data tranmission')
    ser.close()
    ser.open()

while(video_capture.isOpened()):
    
    start = time.time()
    
    ret, frame = video_capture.read()

    crop_img = rescale_frame(frame, percent)
      
    # Convert the img to grayscale 
    gray = cv.cvtColor(crop_img,cv.COLOR_BGR2GRAY)
    
    #Normal Thresholding
    _, thresh = cv.threshold(gray, 100, 255,cv.THRESH_BINARY_INV)
      
    # Apply edge detection method on the image 
    edges = cv.Canny(thresh.copy(),50,150,apertureSize = 3) 
      
    # This returns an array of r and theta values 
    lines = cv.HoughLines(edges,1,np.pi/180, 100) 
    
    if lines is not None:
        # The below for loop runs till r and theta values  
        # are in the range of the 2d array 
        for r,theta in lines[0]: 
            
            # Stores the value of cos(theta) in a 
            a = np.cos(theta) 
          
            # Stores the value of sin(theta) in b 
            b = np.sin(theta) 
              
            # x0 stores the value rcos(theta) 
            x0 = a*r 
              
            # y0 stores the value rsin(theta) 
            y0 = b*r 
              
            # x1 stores the rounded off value of (rcos(theta)-1000sin(theta)) 
            x1 = int(x0 + 1000*(-b)) 
              
            # y1 stores the rounded off value of (rsin(theta)+1000cos(theta)) 
            y1 = int(y0 + 1000*(a)) 
          
            # x2 stores the rounded off value of (rcos(theta)+1000sin(theta)) 
            x2 = int(x0 - 1000*(-b)) 
              
            # y2 stores the rounded off value of (rsin(theta)-1000cos(theta)) 
            y2 = int(y0 - 1000*(a)) 
              
            # cv2.line draws a line in img from the point(x1,y1) to (x2,y2). 
            # (0,0,255) denotes the colour of the line to be  
            #drawn. In this case, it is red.  
            cv.line(crop_img,(x1,y1), (x2,y2), (0,0,255), 4) 
            # radian into degree 
            theta_degree = (theta/22)*180*7
            if theta_degree <= 90 and theta_degree >= 0:
                angle = theta_degree
            elif theta_degree > 90 and theta_degree <=180:
                angle = theta_degree - 180
            else:
                angle = 999
            
            #print('angle = '+ str(angle))
    
    angle_int = int(angle)
    #print('angle_int = '+str(angle_int))
    angle_str = str(angle_int)
    print('angle_str = '+ str(angle_str))
    data_transmission(angle_str)
    
             
    show_graphics() #Calling show_graphics() function
    
    
    end = time.time()
    print("time execution " + str(end-start))
    print("frames per sec " + str(1/(end-start)))

    if cv.waitKey(1) & 0x77 == ord('q'):
        break

#video_capture.release()
#out.release()
#out1.release()
video_capture.release()
cv.destroyAllWindows()
