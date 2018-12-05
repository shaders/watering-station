#ifndef MAIN_WATER_H_INCLUDED
#define MAIN_WATER_H_INCLUDED

#include "State.h"
#include "libs/GravityPump/GravityPump.h"
#include "Valve.h"
#include "WaterOnFloorDetector.h"
#include "Button.h"
#include "WateringState.h"
#include "NutritionState.h"
#include "WaterLevel.h"

#include "MainSensors.h"

const int MAX_STATE = 7;

class MainWater
{
  Button waterPumpButton;
  Button changeSolenoidButton;

  Valve waterValve;
  Valve nutrition1Valve;
  Valve nutrition2Valve;
  Valve nutrition3Valve;

  WaterOnFloorDetector waterOnFloorDetector;
  WaterLevel waterLevelOverflow;
  AnalogWaterLevel waterLevelStartProgram;
  WaterLevel waterLevelMainBucketLow;

  //GravityPump precisePump;

  // for initial setup of the system. solenoid override.
  int activeOpenSolenoid = 0;

  State * state[MAX_STATE];

  const int STATE_IDLE = -1;
  int currentState = STATE_IDLE;

  MainSensors mainSensors;

public:
  MainWater();

  void setup();
  void closeAllValves();
  void loop();
};

#endif // MAIN_WATER_H_INCLUDED
