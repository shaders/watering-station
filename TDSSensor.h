#ifndef TDS_SENSOR_INCLUDED_H
#define TDS_SENSOR_INCLUDED_H

// number of sample point
#define TDS_SAMPLES_COUNT 30

class TDSSensor
{
  int pin;

  int buffer[TDS_SAMPLES_COUNT];
  int index;

  float tdsValue;
  float temperature;

  float calculateTDS();

public:
  TDSSensor(int pin);

  void setup();
  void updateTemperature(float temperature);
  
  float readTDSValue();
};

#endif // TDS_SENSOR_INCLUDED_H
