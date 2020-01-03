# Python3 remaining all the images of a folder
  
# importing os module 
import os 
import cv2
import numpy as np
import time
  
# Function to rename multiple files 
def main():
    
    scale_percent = 95 # percent of original size

    for filename in os.listdir("Photos"):

        img = cv2.imread(os.path.join("Photos",filename))
        height = np.size(img, 0)
        width = np.size(img, 1)
        original_size=(height*width)/(8*1000)
        print(filename+' '+str(original_size))

        size = original_size
        resize_img = img

        while not(size < 300):
            
            width = int(resize_img.shape[1] * scale_percent / 100)
            height = int(resize_img.shape[0] * scale_percent / 100)
            dim = (width, height)
            size = (height*width)/(8*1000)
            # resize image
            resize_img = cv2.resize(img, dim, interpolation = cv2.INTER_AREA)
            print(filename+'Resizing'+str(size))
        
        cv2.imwrite("small"+filename,resize_img)    
  
if __name__ == '__main__': 
    main() 
