import RPi.GPIO as GPIO
import time

GPIO.setwarnings(False)

GPIO.setmode(GPIO.BOARD)
ra1 = 31;
ra2 = 32;
ra3 = 33;
ra4 = 35;

ra5 = 36;
ra6 = 37;
ra7 = 38;
ra8 = 40;

GPIO.setup(pin,GPIO.OUT)

GPIO.output(pin,GPIO.LOW)


GPIO.setup(pin, GPIO.IN)
state = GPIO.input(pin)
