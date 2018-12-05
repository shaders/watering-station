#ifndef PH_SENSOR_INCLUDED_H
#define PH_SENSOR_INCLUDED_H

class PHSensor
{
  int pin;
public:
  PHSensor(int pin)
  : pin(pin)
  {}

  void setup()
  {}

  float readPHValue();
};

#endif // PH_SENSOR_INCLUDED_H
