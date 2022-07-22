from traffic_light import TrafficLight

class TrafficState:
    PHASES = [
        TrafficLight(red=True, yellow=False, green=False),  # __counter = 0 => This is the start state
        TrafficLight(red=True, yellow=True, green=False),
        TrafficLight(red=False, yellow=False, green=True),
        TrafficLight(red=False, yellow=True, green=False),
    ]
    
    def __init__(self, counter: int = 0):
        self.__counter = counter
        
    def thisPhase(self) -> TrafficLight:
        return self.PHASES[self.__counter]
    
    def nextPhase(self):
        self.__counter = (self.__counter + 1) % len(self.PHASES)