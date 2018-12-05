#ifndef WATER_ON_FLOOR_DETECTOR_H_INCLUDED
#define WATER_ON_FLOOR_DETECTOR_H_INCLUDED

class WaterOnFloorDetector
{
  int pin;

  const int SENSOR_TOUCHES_WATER = 700;
  
public:
  WaterOnFloorDetector(int pin)
  : pin(pin)
  {}

  void setup()
  {}

  bool isWaterOnFloor()
  {
    // read the input on analog pin:
    int moisture = analogRead(pin);
    if(moisture < SENSOR_TOUCHES_WATER)
      return true;
  
    return false;
  }
};

#endif // WATER_ON_FLOOR_DETECTOR_H_INCLUDED
