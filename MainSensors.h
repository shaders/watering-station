#ifndef MAIN_SENSORS_H_INCLUDED
#define MAIN_SENSORS_H_INCLUDED

#include <Wire.h>
#include "libs/HttpClient/HttpClient.h"
#include "libs/WiFiEsp/WiFiEsp.h"
#include "SoftwareSerial.h"

#include "StringFormatter.h"
#include "Display.h"
#include "AmbientLightSensor.h"
#include "PHSensor.h"
#include "TDSSensor.h"
#include "BarometerSensor.h"
#include "libs/DHT-sensor/DHT_U.h"

#include "libs/GravityRtc/GravityRtc.h"

class MainSensors
{
  //temperature and humidity sensor (DHT11 model)
  DHT_Unified temperatureSensor;
  AmbientLightSensor lights;
  PHSensor phSensor;
  TDSSensor tdsSensor;
  BarometerSensor barometer;

  Display lcd;
  GravityRtc realTimeClock;

  SoftwareSerial softSerial;

  // Initialize the Ethernet client object
  WiFiEspClient client;

  void printWifiStatus();

public:
  MainSensors();

  void setupSensorsAndWiFi();
  void sendSensorsData(int waterOnFloor, int highWater, int lowMainWater);

  Display * getLCD()
  {
    return &lcd;
  }
};

#endif // MAIN_SENSORS_H_INCLUDED
