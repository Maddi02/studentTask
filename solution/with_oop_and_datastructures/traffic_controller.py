from dataclasses import dataclass

from traffic_state import TrafficState
from light_controller import LightController
from RPi_led_controller import RPiLedController


@dataclass
class TrafficController:
    state: TrafficState = TrafficState()
    controller: LightController = RPiLedController()
    
    def showPhase(self):
        self.controller.apply(self.state.thisPhase())
        
    def nextPhase(self):
        self.state.nextPhase()