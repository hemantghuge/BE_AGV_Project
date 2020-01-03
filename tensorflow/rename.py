# Python3 remaining all the images of a folder
  
# importing os module 
import os 
  
# Function to rename multiple files 
def main(): 
    i = 1
      
    for filename in os.listdir("Photos"): #"Photos" - Name of folder
        dst ="image" + str(i) + ".jpg"
        src ="Photos/"+ filename 
        dst ="Photos/"+ dst 
          
        # rename() function will rename all the files 
        os.rename(src, dst) 
        i += 1
  
if __name__ == '__main__': 
    main() 
