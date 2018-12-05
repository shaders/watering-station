#ifndef AMBIENT_LIGHT_SENSOR_INCLUDED_H
#define AMBIENT_LIGHT_SENSOR_INCLUDED_H

#include "AnalogSensor.h"

class AmbientLightSensor : public AnalogSensor
{
public:
  AmbientLightSensor(int pin)
  : AnalogSensor(pin)
  {}

  // will return from 0 (dark) to 100 (bright)
  virtual float getValue()
  {
    float val = AnalogSensor::getValue();
    return val * 100.0 / 1024.0;
  }
};

#endif // AMBIENT_LIGHT_SENSOR_INCLUDED_H
