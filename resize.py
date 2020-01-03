# Python3 remaining all the images of a folder
# WARNING - Exisitng photos will be overwritten so make sure to have a backup folder
  
# importing os module 
import os 
import cv2
import numpy as np
import time
  
# Function to rename multiple files 
def main(): 
    scale_percent = 95 # percent of original size
      
    for filename in os.listdir("Photos"):

        img = cv2.imread(os.path.join("Photos",filename)) #reading images from the folder
        height = np.size(img, 0) #height of image
        width = np.size(img, 1) #width of image
        
        original_size = os.path.getsize(os.path.join("Photos",filename))#calculating size in bytes
        original_size = original_size/1024  #converting into kilo bytes
        print(filename+' '+str(original_size)) 

        size = original_size 
        resize_img = img

        while not(size < 300):
            
            width = int(resize_img.shape[1] * scale_percent / 100) #finding width value
            height = int(resize_img.shape[0] * scale_percent / 100) #finding height value
            dim = (width, height)
            size = os.path.getsize(os.path.join("Photos",filename)) #calculating size in bytes
            size = size/1024;   #converting into kilo bytes
            # resize image
            resize_img = cv2.resize(img, dim, interpolation = cv2.INTER_AREA)
            print(filename+' Resizing '+str(size))
            cv2.imwrite("Photos/"+filename,resize_img)  #saving the image

if __name__ == '__main__': 
    main() 
