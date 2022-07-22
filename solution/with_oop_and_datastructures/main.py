#!/usr/bin/python3

import time

from traffic_controller import TrafficController

if __name__ == '__main__':
    controller = TrafficController()
    
    while True:
        controller.showPhase()
        time.sleep(2)
        controller.nextPhase()