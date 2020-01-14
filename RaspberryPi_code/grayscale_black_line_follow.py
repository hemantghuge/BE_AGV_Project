import numpy as np
import cv2 as cv
import time

width = 640
height = 480
fps = 30

video_capture = cv.VideoCapture(0)
video_capture.set(3,width)
video_capture.set(4,height)
video_capture.set(5,fps)

fourcc = cv.VideoWriter_fourcc(*'XVID')
video_capture.set(cv.CAP_PROP_FOURCC,cv.VideoWriter_fourcc(*'MJPG'))


while(video_capture.isOpened()):
    
    start = time.time()
    
    ret, frame = video_capture.read()

    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)

    cv.imshow('frame', frame)
    cv.imshow('gray', gray)

    if cv.waitKey(1) & 0x77 == ord('q'):
        break

video_capture.release()
cv.destroyAllWindows()
