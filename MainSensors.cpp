#include "MainSensors.h"

const char *WIFI_SSID     = ""; // Enter your Wi-Fi name
const char *WIFI_PASSWORD = "";  // Enter your Wi-Fi password

// Pin Definitions
#define WIFI_PIN_RX 10
#define WIFI_PIN_TX 11

MainSensors::MainSensors()
: temperatureSensor(12, DHT11),
  lights(A2),
  phSensor(A3),
  tdsSensor(A1),
  lcd(0x20, 20, 4),  // set the LCD address to 0x20 for a 20 chars and 4 line display (All jumpers should be connected!)
  softSerial(WIFI_PIN_RX, WIFI_PIN_TX) // RX, TX
{}

void MainSensors::printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void MainSensors::setupSensorsAndWiFi()
{
  // initialize serial for ESP module
  softSerial.begin(9600);
  // initialize ESP module
  WiFi.init(&softSerial);

  // check for the presence of the Wifi
  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println("WiFi is not present");
    // don't continue
    while (true);
  }

  int status = WL_IDLE_STATUS;
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_SSID);

    // Connect to WPA/WPA2 network
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  printWifiStatus();

  Serial.println("Setup LCD");
  lcd.setup();

  Serial.println("Setup RTC");
  realTimeClock.setup();

  Serial.println("Setup Temperature");
  temperatureSensor.begin();

  Serial.println("Setup Lights");
  lights.setup();

  Serial.println("Setup TDS");
  tdsSensor.setup();

  Serial.println("Setup Barometer");
  barometer.setup();

  Serial.println("Finished WiFi and Sensors setup");
}

void MainSensors::sendSensorsData(int waterOnFloor, int highWater, int lowMainWater)
{  
  realTimeClock.read();

  lcd.home();

  String dateNow = StringFormatter::formatString(F("Today: %d/%d/%d      "), realTimeClock.year, realTimeClock.month, realTimeClock.day);
  lcd.setCursor(0, 0);
  lcd.print(dateNow);

  String timeNow = StringFormatter::formatString(F("Time: %d:%d:%d      "), realTimeClock.hour, realTimeClock.minute, realTimeClock.second);
  lcd.setCursor(0, 1);
  lcd.print(timeNow);
  
  sensors_event_t temp;
  temperatureSensor.temperature().getEvent(&temp);
  if (isnan(temp.temperature)) {
    Serial.println(F("Error reading temperature!"));
    temp.temperature = -100;
  }

  sensors_event_t humid;
  // Get humidity event and print its value.
  temperatureSensor.humidity().getEvent(&humid);
  if (isnan(humid.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
    humid.relative_humidity = -100;
  }

  float pressure = barometer.getPressure();
  Serial.print(F("Pressure: "));
  Serial.println(pressure);

  char convBuffer[64];
  char convBuffer2[64];
  //String tempStr = StringFormatter::formatString(F("T %dC H %d%%"), int(temp.temperature), int(humid.relative_humidity));
  String tempStr = StringFormatter::formatString(F("T %dC H %d%% P %s"), int(temp.temperature), int(humid.relative_humidity), dtostrf(pressure, 3, 1, convBuffer));
  lcd.setCursor(0, 2);
  lcd.print(tempStr);

  if (temp.temperature >= 0)
    tdsSensor.updateTemperature(temp.temperature);

  float tdsValue = tdsSensor.readTDSValue();
  float phValue = phSensor.readPHValue();

  String phStr = StringFormatter::formatString(F("PH: %s TDS: %s"), dtostrf(phValue, 3, 2, convBuffer), dtostrf(tdsValue, 3, 2, convBuffer2));
  lcd.setCursor(0, 3);
  lcd.print(phStr);

  float lightVal = lights.getValue();
  Serial.print(F("Light: "));
  Serial.println(lightVal);

  //do not send data too often
  bool sendData = false;
  
  static unsigned long prevTime = 0;
  if(!prevTime)
  {
    prevTime = millis();
    //send data first time
    sendData = true;
  }
    
  unsigned long newTime = millis();

  long deltaTime = newTime - prevTime;
  if(deltaTime < 0)
  {
    // millis overflowed. reset and send data anyway.
    prevTime = newTime = millis();
    sendData = true;
  }

  // send data once every 60 seconds
  if(deltaTime/1000 >= 60)
    sendData = true;

  if(!sendData)
    return;

  //send data to Cloud
  HttpClient http(client);
  http.beginRequest();

  int err = http.startRequest("34.229.106.54", 8086, "/write?db=sensors", "POST", NULL);
  http.sendHeader("Content-Type", "text/plain");
  http.sendBasicAuth("arduino", "P@ssw0rd!$88");

  String body = "sensors,room=DC ";
  String tStr = StringFormatter::formatString(F("temperature=%d,humidity=%d,pressure=%s"), int(temp.temperature), int(humid.relative_humidity), dtostrf(pressure, 3, 1, convBuffer));
  //String tStr = StringFormatter::formatString(F("temperature=%d,humidity=%d"), int(temp.temperature), int(humid.relative_humidity));
  body += tStr;

  tStr = StringFormatter::formatString(F(",ph=%s,tds=%s"), dtostrf(phValue, 3, 2, convBuffer), dtostrf(tdsValue, 3, 2, convBuffer2));
  body += tStr;

  tStr = StringFormatter::formatString(F(",light=%s"), dtostrf(lightVal, 3, 2, convBuffer));
  body += tStr;

  tStr = StringFormatter::formatString(F(",waterOnFloor=%d,waterOverflow=%d,lowMainWater=%d"), waterOnFloor, highWater, lowMainWater);
  body += tStr;

  //  Serial.println(body);

  http.sendHeader("Content-length", body.length());
  http.write((const uint8_t*)body.c_str(), body.length());
  http.endRequest();

  if (err != 0)
  {
    Serial.println("Failed to send data");
  }
  else
  {
    int retCode = http.responseStatusCode();
    Serial.print("HTTP Status code: ");
    Serial.println(retCode);
  }
  http.stop();

  // recording new time
  prevTime = millis();
}
