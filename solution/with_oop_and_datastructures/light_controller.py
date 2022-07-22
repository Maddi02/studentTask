from traffic_light import TrafficLight

class LightController:
    """
    Interface that turns the lights off/on.
    """
    def apply(self, tl: TrafficLight):
        """Turns the lights of a traffic off/on according to tl.
        Args:
            tl (TrafficLight): The traffic light to show
        """
        pass