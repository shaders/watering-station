#include "PHSensor.h"
#include <Arduino.h>

float PHSensor::readPHValue()
{
  //Get 10 sample value from the sensor for smooth the value
  int buf[10];
  for(int i = 0; i < 10; i++)
  {
    buf[i] = analogRead(pin);
    delay(10);
  }

  //sort the analog from small to large
  for(int i = 0; i < 9; i++)
  {
    for(int j = i + 1; j < 10; j++)
    {
      if(buf[i] > buf[j])
      {
        int temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }

  //take the average value of 6 center sample
  float avgValue = 0;
  for(int i = 2; i < 8; i++)
    avgValue += buf[i];

  avgValue /= 6.0;

  //convert the analog into millivolt
  float phValue = avgValue*5.0/1024.0;

  //convert the millivolt into pH value
  phValue = 3.5 * phValue;

  return phValue;
}
