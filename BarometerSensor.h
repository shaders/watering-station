#ifndef BAROMETER_SENSOR_INCLUDED_H
#define BAROMETER_SENSOR_INCLUDED_H

#include "libs/SFE_BMP180/SFE_BMP180.h"

class BarometerSensor
{
  SFE_BMP180 sensor;
  double pressure;
  double temperature;

  void getSensorReadings();

public:
  BarometerSensor();
  void setup();  
  float getPressure();
};

#endif // BAROMETER_SENSOR_INCLUDED_H
