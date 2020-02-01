#Parallel communication between Arduino Mega and Raspberry Pi
import RPi.GPIO as GPIO #importing GPIO Python Package
import time

GPIO.setwarnings(False) #disable GPIO related warning

GPIO.setmode(GPIO.BOARD) #GPIO mode is set to BOARD and not to BCM

#ra = raspi to arduino mega data transmission
ra1 = 31 
ra2 = 32
ra3 = 33
ra4 = 35

ra5 = 36
ra6 = 37
ra7 = 38
ra8 = 40

#ar = arduino mega to raspi data transmission 
ar1 = 13
ar2 = 15
ar3 = 16
ar4 = 18

#Enabling raspi to arduino mega pins as OUTPUT
GPIO.setup(ra1,GPIO.OUT)
GPIO.setup(ra2,GPIO.OUT)
GPIO.setup(ra3,GPIO.OUT)
GPIO.setup(ra4,GPIO.OUT)

GPIO.setup(ra5,GPIO.OUT)
GPIO.setup(ra6,GPIO.OUT)
GPIO.setup(ra7,GPIO.OUT)
GPIO.setup(ra8,GPIO.OUT)

#Enabling arduino mega to raspi pins as INPUT
GPIO.setup(ar1,GPIO.IN)
GPIO.setup(ar2,GPIO.IN)
GPIO.setup(ar3,GPIO.IN)
GPIO.setup(ar4,GPIO.IN)

#storing logic in signal_name variable
signal_value = GPIO.HIGH 

while True:

	GPIO.output(ra1,signal_value)
	GPIO.output(ra2,signal_value)
	GPIO.output(ra3,signal_value)
	GPIO.output(ra4,signal_value)
	
	GPIO.output(ra5,signal_value)
	GPIO.output(ra6,signal_value)
	GPIO.output(ra7,signal_value)
	GPIO.output(ra8,signal_value)
	
	ar1State = GPIO.input(ar1)
	ar2State = GPIO.input(ar2)
	ar3State = GPIO.input(ar3)
	ar4State = GPIO.input(ar4)
	
	print(" ar1State = ")
	print(ar1State)
	print(" ar2State = ")
	print(ar2State)
	print(" ar3State = ")
	print(ar3State)
	print(" ar4State = ")
	print(ar4State)
	print("\n")
	
