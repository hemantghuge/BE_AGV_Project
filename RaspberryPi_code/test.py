import RPi.GPIO as GPIO #importing GPIO Python Package
import time

GPIO.setwarnings(False)

GPIO.setmode(GPIO.BOARD) #GPIO mode is set to BOARD and not to BCM
pin = 10; #change number for testing all GPIO of Raspberry Pi

#Raspberry Pi is 3.3V compatible so it's max voltage is 3.3V

# enabling GPIO as OUTPUT
GPIO.setup(pin,GPIO.OUT)
GPIO.output(pin,GPIO.LOW)

# enabling GPIO as INPUT
GPIO.setup(pin, GPIO.IN)
state = GPIO.input(pin)
