import numpy as np
import cv2 as cv
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
out = cv.VideoWriter('videoAfterProcessing.avi',fourcc, fps, (width,height))
out1 = cv.VideoWriter('videoThreshold.avi', fourcc, fps, (width,height))
video_capture.set(cv.CAP_PROP_FOURCC,cv.VideoWriter_fourcc(*'MJPG'))


def rescale_frame(frame, percent):
    width = int(frame.shape[1]*percent/100)
    height = int(frame.shape[0]*percent/100)
    dim = (width, height)
    return cv.resize(frame, dim, interpolation = cv.INTER_AREA)

def show_graphics():
    cv.imshow('crop_img',crop_img)
    cv.imshow('thresh', thresh)
    return 0

while(video_capture.isOpened()):
    
    start = time.time()
    
    ret, frame = video_capture.read()

    crop_img = rescale_frame(frame, percent)

    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    
    _, thresh = cv.threshold(gray, 100, 255,cv.THRESH_BINARY_INV)

    _, contours, _ = cv.findContours(thresh.copy(), 1, cv.CHAIN_APPROX_NONE)

    if len(contours) > 0:
        con = max(contours, key = cv.contourArea)
        area = cv.contourArea(con)
        print('area = ' + str(area))
        
        M = cv.moments(con)

        if M['m00'] == 0:
            M['m00'] = 0.0000001;

        cx = int(M['m10']/M['m00'])
        cy = int(M['m01']/M['m00'])

        cv.line(crop_img,(cx,0),(cx,height), (255,0,0),3)
        cv.line(crop_img,(0,cy),(width,cy), (255,0,0),3)

        cv.drawContours(crop_img, con, -1, (0,255,0), 3) #con=max(contours)

        if cx <= width/3:
            print ('left')

        elif cx > width/3 and cx < width*(2/3):
            print ('On Track')

        elif cx >= width*(2/3):
            print ('Right')
        
        else:
            print('Nothing')
            
    else:
        print ('I don\'t see the contour') 

    show_graphics()
    
    out.write(crop_img)
    out1.write(thresh)

    end = time.time()
    print("time execution " + str(end-start))
    print("frames per sec " + str(1/(end-start)))

    if cv.waitKey(1) & 0x77 == ord('q'):
        break

#video_capture.release()
out.release()
out1.release()
cv.destroyAllWindows()
