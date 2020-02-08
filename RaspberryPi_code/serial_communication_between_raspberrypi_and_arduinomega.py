import serial
import RPi.GPIO as GPIO
import time,sys

SERIAL_PORT = "/dev/ttyS0"
ser = serial.Serial(SERIAL_PORT, baudrate = 9600)

a = '90'
b = '5'
c = '7'

ser.write(b'100')
print('ninty')
ser.close()
ser.open()
ser.write(b.encode())
print('five')
ser.close()
ser.open()
ser.write(c.encode())
print('seven')
ser.close()
