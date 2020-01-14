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





