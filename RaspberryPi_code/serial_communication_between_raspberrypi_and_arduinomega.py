import serial
import RPi.GPIO as GPIO
import time,sys

SERIAL_PORT = "/dev/ttyS0"
ser = serial.Serial(SERIAL_PORT, baudrate = 115200)
ser.close()

# 0 - negative
# 1 - positive

angle = 0

qr = 'AGV'

x_pos=3 #range(1-5)

# if angle < 0:
#     sign = 0
#     angle = -angle
# else:
#     sign = 1
#     angle = angle

#data = '%'+str(angle)+'%'+str(sign)+'%'+str(qr)+'%'+str(x_pos)+'%'+'end'
data = '%'+str(angle)+'%'+str(qr)+'%'+str(x_pos)+'%'+'end'

ser.open()
ser.write(data.encode())
print(data)
ser.close()

# angle_str = str(int(angle))
# sign_str = str(int(sign))
# x_pos_str = str(int(x_pos))
# 
# ser.open()
# ser.write(angle_str.encode())
# print(angle_str)
# ser.close()
# 
# ser.open()
# ser.write(sign_str.encode())
# print(sign_str)
# ser.flush()
# 
# ser.write(qr.encode())
# print(qr)
# ser.close()
# 
# ser.open()
# ser.write(x_pos_str.encode())
# print(x_pos)
# ser.close()