from enum import Enum

import RPi.GPIO as GPIO

from light_controller import LightController
from traffic_light import TrafficLight
from light_controller import LightController

class TrafficLightChannel(Enum):
    RED = 7
    YELLOW = 5
    GREEN = 3

class RPiLedController(LightController):
    def __init__(self):
        GPIO.setwarnings(False)
        GPIO.cleanup()
        GPIO.setmode(GPIO.BOARD)
        GPIO.setup(TrafficLightChannel.RED.value, GPIO.OUT)
        GPIO.setup(TrafficLightChannel.YELLOW.value, GPIO.OUT)
        GPIO.setup(TrafficLightChannel.GREEN.value, GPIO.OUT)
    
    def apply(self, tl: TrafficLight):
        self.__adaptLight(tl.red, TrafficLightChannel.RED)
        self.__adaptLight(tl.yellow, TrafficLightChannel.YELLOW)
        self.__adaptLight(tl.green, TrafficLightChannel.GREEN)
        
    def __adaptLight(self, on: bool, channel: TrafficLightChannel):
        requiredSignal = GPIO.HIGH if on else GPIO.LOW
        
        if GPIO.input(channel.value) == requiredSignal:
            # we have nothing to do
            return
        
        GPIO.output(channel.value, requiredSignal)