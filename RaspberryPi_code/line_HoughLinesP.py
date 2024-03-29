# Python program to illustrate HoughLine 
# method for line detection 
import cv2 as cv
import numpy as np 
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


def rescale_frame(frame, percent):
    width = int(frame.shape[1]*percent/100)
    height = int(frame.shape[0]*percent/100)
    dim = (width, height)
    return cv.resize(frame, dim, interpolation = cv.INTER_AREA)

def show_graphics():
    cv.imshow('crop_img',crop_img)
    cv.imshow('edges', edges)
    return 0

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
    lines = cv.HoughLinesP(edges,1,np.pi/180, 100, minLineLength = 100, maxLineGap = 10) 
    
    print(lines)
    
    if lines is not None:
        # The below for loop runs till r and theta values  
        # are in the range of the 2d array 
        for line in lines[0]: 
            
            x1, y1, x2, y2 = line
            
            theta = np.arctan((y2-y1)/(x2-x1))    
            # cv2.line draws a line in img from the point(x1,y1) to (x2,y2). 
            # (0,0,255) denotes the colour of the line to be  
            #drawn. In this case, it is red.  
            cv.line(crop_img,(x1,y1), (x2,y2), (0,0,255),2) 

            theta_degree = (theta/22)*180*7
            
            if theta_degree > 0:
                angle = theta_degree - 90
            elif theta_degree < 0:
                angle = theta_degree + 90
            else:
                angle = theta_degree
            
            print('angle = '+ str(angle))
              
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