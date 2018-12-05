#include "TDSSensor.h"
#include <Arduino.h>

TDSSensor::TDSSensor(int pin)
: pin(pin),
  index(0),
  tdsValue(-1),
  temperature(25)
{}

void TDSSensor::setup()
{
  pinMode(pin, INPUT);
  index = 0;
  tdsValue = -1;
  temperature = 25;
}

void TDSSensor::updateTemperature(float temperature)
{
  this->temperature = temperature;
}

float TDSSensor::readTDSValue()
{
  //every 40 milliseconds,read the analog value from the ADC
  //we are going to spend 400 ms here to get 10 samples
  for(int i = 0; i < 10; ++i)
  {
    //read the analog value and store into the buffer
    buffer[index++] = analogRead(pin);
    delay(40);
     
    if(index == TDS_SAMPLES_COUNT)
    {
      index = 0;
      calculateTDS();
      break;
    }
  }

  return tdsValue;
}

float TDSSensor::calculateTDS()
{
  //sort the analog from small to large
  for(int i = 0; i < 9; i++)
  {
    for(int j = i + 1; j < 10; j++)
    {
      if(buffer[i] > buffer[j])
      {
        int temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;
      }
    }
  }

  //take the average value of 10 center sample
  float avgValue = 0;
  for(int i = 10; i < 20; i++)
    avgValue += buffer[i];

  avgValue /= 10;

  //convert the analog into millivolt
  float avgVoltage = avgValue * 5.0 / 1024.0;

  //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
  float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);

  //temperature compensation
  float compensationVolatge = avgVoltage / compensationCoefficient;

  //convert voltage value to tds value
  tdsValue = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5;
}
