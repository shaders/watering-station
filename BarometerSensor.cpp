#include "BarometerSensor.h"

BarometerSensor::BarometerSensor()
: temperature(0),
  pressure(0)
{}

void BarometerSensor::setup()
{
  if (sensor.begin())
  {
    Serial.println(F("BMP180 init success"));
  }
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.
    Serial.println(F("BMP180 init fail (disconnected?)\n\n"));
  }

  getSensorReadings();
}

float BarometerSensor::getPressure()
{
  getSensorReadings();
  return pressure;
}

void BarometerSensor::getSensorReadings()
{  
  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  int status = sensor.startTemperature();
  if (status == 0)
  {
    Serial.println(F("error starting temperature measurement\n"));
    return;
  }

  // Wait for the measurement to complete:
  delay(status);

  // Retrieve the completed temperature measurement:
  status = sensor.getTemperature(temperature);
  if (status == 0)
  {
    Serial.println(F("error retrieving temperature measurement\n"));
    return;
  }

  // Start a pressure measurement:
  // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.
  status = sensor.startPressure(1);
  if (status == 0)
  {
    Serial.println(F("error starting pressure measurement\n"));
    return;
  }

  // Wait for the measurement to complete:
  delay(status);

  // Retrieve the completed pressure measurement:
  // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
  // Function returns 1 if successful, 0 if failure.
  status = sensor.getPressure(pressure, temperature);
  if (status == 0)
  {
    Serial.println(F("error retrieving pressure measurement\n"));
    return;
  }
}
