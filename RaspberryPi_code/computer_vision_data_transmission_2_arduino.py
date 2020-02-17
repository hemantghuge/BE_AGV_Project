# Post computer vision data to Arduino Mega
# 8 pins will be used for data transmission
# Data will be transferred using Parallel Communications

import numpy as np 
import cv2 as cv
import pyzbar.pyzbar as pyzbar
import time

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

area_threshold = 15000


def rescale_frame(frame, percent):
    width = int(frame.shape[1]*percent/100)
    height = int(frame.shape[0]*percent/100)
    dim = (width, height)
    return cv.resize(frame, dim, interpolation = cv.INTER_AREA)

def show_graphics():
    cv.imshow('crop_img',crop_img)
    cv.imshow('thresh', thresh)
    return 0

def calculate_angle(points):
            
    blackbox = cv.minAreaRect(points)
    #print('blackbox= '+ str(blackbox))
    (x_min, y_min), (w_min, h_min), ang = blackbox

    if ang < -45 :
        ang = 90 + ang
          
    if w_min < h_min and ang > 0:    
        ang = (90-ang)*-1
                      
    if w_min > h_min and ang < 0:
        ang = 90 + ang
            
    ang = int(ang)
    print('ang = '+ str(ang))
    cv.putText(crop_img, 'Angle ='+ str(ang),(400,50), cv.FONT_HERSHEY_SIMPLEX, 1, (20, 20, 250), 3)
        
    return(ang)

def line_follow(cx):
    if cx <= width/3:
        print ('left')
    elif cx > width/3 and cx < width*(2/3):
        print ('On Track')
    elif cx >= width*(2/3):
        print ('Right')
    else:
        print('x-axis point not in frame')
    #return flag    
    
        
def transfer_Angle(ang):
    print('In Transfer Angle Function')


while(video_capture.isOpened()):
    
    start = time.time()
    
    ret, frame = video_capture.read()

    crop_img = rescale_frame(frame, percent)
    
    decodedObjects = pyzbar.decode(crop_img)
    for obj in decodedObjects:
        print('QR FOUND')
        cv.putText(crop_img, str(obj.data), (50, 50), font, 2, (255, 0, 0), 3)  #inserting text on the frame
        time.sleep(5)
    
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
            
            ang = calculate_angle(con)
            
            M = cv.moments(con)

            if M['m00'] == 0:
                M['m00'] = 0.0000001;

            #cx -> center point of x-axis
            cx = int(M['m10']/M['m00'])
            #cy -> center point of y-axis
            cy = int(M['m01']/M['m00'])

            cv.line(crop_img,(cx,0),(cx,height), (255,0,0),3)
            cv.line(crop_img,(0,cy),(width,cy), (255,0,0),3)

            cv.drawContours(crop_img, con, -1, (0,255,0), 3) #con=max(contours)
            
            flag_line = line_follow(cx)
            
            #transfer Angle value to through GPIO
            transfer_Angle(ang)
            
        else:
            print('Area less than threshold value i.e.' + str(area_threshold))
            
    else:
        print ('I don\'t see the contour')        

    show_graphics() #Calling show_graphics() function
    
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


