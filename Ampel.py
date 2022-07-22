#!/usr/bin/python3

import RPi.GPIO as GPIO
import time
GPIO.setwarnings(False)
GPIO.cleanup()
GPIO.setmode(GPIO.BOARD)
GPIO.setup(7,GPIO.OUT)
GPIO.setup(5,GPIO.OUT)
GPIO.setup(3,GPIO.OUT)

while True: 
	GPIO.output(7,GPIO.HIGH) #Red Light is on
	time.sleep(2) # remains there for two seconds
	GPIO.output(5, GPIO.HIGH) # Yellow Light turns on
	time.sleep(2) # remains in this state for 2 seconds
	GPIO.output(7, GPIO.LOW) # Red Light  shout down
	GPIO.output(5, GPIO.LOW) # Yellow Light shout down
	GPIO.output(3, GPIO.HIGH) # Green Light turns on
	time.sleep(2) # stay there for 2 seconds
	GPIO.output(3, GPIO.LOW) # Green light turns of
	GPIO.output(5, GPIO.HIGH)
	time.sleep(1)
	GPIO.output(5, GPIO.LOW)
	#GPIO.output(5, GPIO.HIGH)
	#GPIO.output(5,GPIO.LOW)
	#time.sleep(1)

