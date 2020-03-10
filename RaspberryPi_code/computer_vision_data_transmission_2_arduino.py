# Post computer vision data to Arduino Mega
# 8 pins will be used for data transmission
# Data will be transferred using Parallel Communications

import numpy as np 
import cv2 as cv
import serial
import pyzbar.pyzbar as pyzbar
import time, sys

SERIAL_PORT = "/dev/ttyS0"
ser = serial.Serial(SERIAL_PORT, baudrate = 115200)
ser.close()

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

area_threshold = 18000

ang = 0
ang_value = 0
flag_line = 3


def rescale_frame(frame, percent):
    width = int(frame.shape[1]*percent/100)
    height = int(frame.shape[0]*percent/100)
    dim = (width, height)
    return cv.resize(frame, dim, interpolation = cv.INTER_AREA)

def calculate_angle(thresh):
    
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
    
            angle_float = round(angle,3)
            #print('angle_int = '+str(angle_int))
            angle_str = str(angle_float)
            
            #print('angle_str = '+ str(angle_str))
            cv.putText(crop_img, 'Angle ='+ str(angle_float),(400, 50), cv.FONT_HERSHEY_SIMPLEX, 1, (20, 220, 250), 4)    
    
        return(angle_float)

def line_follow(cx):
    if cx <= width/5:
        flag = 1
        print ('left')
    
    elif cx > width/5 and cx < width*(2/5):
        flag = 2
        print ('slight left')
    
    elif cx > width*(2/5) and cx < width*(3/5):
        flag = 3
        print ('On Track')
        
    elif cx > width*(3/5) and cx < width*(4/5):
        flag = 4
        print ('slight right')
    
    elif cx >= width*(4/5) and cx < width:
        flag = 5
        print ('right')
    else:
        flag = 3
        
    return flag    
#     else:
#         flag = flag
#         print('x-axis point not in frame')

    
        
def data_transmission(angle, qr, x_pos):
    print('DATA TRANSMISSION')
        
    data = '%'+str(angle)+'%'+str(qr)+'%'+str(x_pos)+'%'+'end'
    
    ser.open()
    ser.write(data.encode())
    print(data)
    ser.close()
    
    print('DATA TRANSMISSION')
    
    cv.imshow('thresh', thresh)
    cv.imshow('crop_img',crop_img)


while(video_capture.isOpened()):
    
    start = time.time()
    
    ret, frame = video_capture.read()

    crop_img = rescale_frame(frame, percent)
    
    decodedObjects = pyzbar.decode(crop_img)
    #print(type(decodedObjects))
    
    if decodedObjects:
        for obj in decodedObjects:
            #print('QR FOUND')
            qr = str(obj.data)
            cv.putText(crop_img, qr, (50, 50), font, 2, (255, 0, 0), 3)  #inserting text on the frame            
            #time.sleep(5)
    else:
        qr = 'No'
        #print('QR NOT FOUND')
        
    print('QR value = ' + str(qr))
    
    #BGR2GRAY conversion
    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    
    #Normal Thresholding
    _, thresh = cv.threshold(gray, 100, 255,cv.THRESH_BINARY_INV)
    
    #Finding Contours
    _, contours, _ = cv.findContours(thresh.copy(), 1, cv.CHAIN_APPROX_NONE)

    if len(contours) > 0:
        con = max(contours, key = cv.contourArea)
        area = cv.contourArea(con)
        print('area = ' + str(area))

        if (area > area_threshold):
            
            M = cv.moments(con)

            if M['m00'] == 0:
                M['m00'] = 0.0000001;

            #cx -> center point of x-axis
            cx = int(M['m10']/M['m00'])
            #cy -> center point of y-axis
            cy = int(M['m01']/M['m00'])

            cv.drawContours(crop_img, con, -1, (0,255,0), 2) #con=max(contours)
            
            cv.line(crop_img,(cx,0),(cx,height), (255,0,0),2)
            cv.line(crop_img,(0,cy),(width,cy), (255,0,0),2)
            
            flag_line = line_follow(cx)
            print('x_pos = '+ str(flag_line))
            
            
            ang = calculate_angle(thresh.copy())
            if ang == None:
                ang = 0
                
            print('ang = ' + str(ang))
            
            
            
            
        else:
            print('Area less than threshold value i.e.' + str(area_threshold))
            
    else:
        print ('I don\'t see the contour')        
    
    ang_value = ang
    
    data_transmission(ang_value, qr, flag_line)
    
    #show_graphics() #Calling show_graphics() function    
    #out.write(crop_img) #saving frame named crop_img
    #out1.write(thresh) #saving frame named thresh

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


