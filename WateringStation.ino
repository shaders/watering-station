#include "MainWater.h"

MainWater mainWater;

void setup() {
  // prepare serial communication
  Serial.begin(115200);
  Wire.begin();

  mainWater.setup();

  Serial.println("Finished setup");
}

void loop() {
  mainWater.loop();
}
