#ifndef ANALOG_SENSOR_INCLUDED_H
#define ANALOG_SENSOR_INCLUDED_H

class AnalogSensor
{
  int pin;
public:
  AnalogSensor(int pin)
  : pin(pin)
  {}

  virtual void setup()
  {}

  // will return from 0 to 1024
  virtual float getValue()
  {
    // get max of 100 values in a tight loop to prevent sensor noise (see on light sensor, didn't investigate enough why)
    // FIXME: need to check and fix on v2.0
    int maxVal = 0;
    for(int i = 0; i < 100; ++i)
    {
      int val = analogRead(pin);  
      if(val > maxVal)
        maxVal = val;
    }

    return maxVal;
  }
};

class AnalogWaterLevel : public AnalogSensor                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
{
public:
  AnalogWaterLevel(int pin)
  : AnalogSensor(pin)
  {}

  bool isInWater()
  {
    float value = getValue();
    if(value > 800)
      return true;

    return false;
  }
};

#endif // ANALOG_SENSOR_INCLUDED_H
