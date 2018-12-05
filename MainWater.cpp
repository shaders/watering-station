#include "MainWater.h"
#include "libs/FlowMeter/FlowMeter.h"

const int flowSensorPin = 2;
// enter your own sensor properties here, including calibration points
static FlowSensorProperties sensorCalibration = {60.0f, 40.0f, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
static FlowMeter flowMeter = FlowMeter(flowSensorPin, sensorCalibration);

// define an 'interrupt service routine' (ISR)
static void pulseCounter()
{
  // let our flow meter count the pulses
  flowMeter.recordPulse();
}

const int waterPumpButtonPin = 4;
const int buttonChangeSolenoidPin = 13; //this one is to prepare the station

const int solenoidPinWater = 8;
const int solenoidPinNutrition1 = 3;
const int solenoidPinNutrition2 = 5;
const int solenoidPinNutrition3 = 6;

const int waterDetectorPin = A0;
const int waterLevelOverflowPin = 32;
const int waterLevelStartProgramPin = A4;
const int waterLevelMainBucketPin = 42;

const int pumpPin = 7;
//const int precisePumpPin = 9;

MainWater::MainWater()
: waterPumpButton(waterPumpButtonPin),
  changeSolenoidButton(buttonChangeSolenoidPin),
  waterValve(solenoidPinWater),
  nutrition1Valve(solenoidPinNutrition1),
  nutrition2Valve(solenoidPinNutrition2),
  nutrition3Valve(solenoidPinNutrition3),
  waterOnFloorDetector(waterDetectorPin),
  waterLevelOverflow(waterLevelOverflowPin),
  waterLevelStartProgram(waterLevelStartProgramPin),
  waterLevelMainBucketLow(waterLevelMainBucketPin)
{}

void MainWater::setup()
{
  mainSensors.setupSensorsAndWiFi();

  waterOnFloorDetector.setup();
  waterLevelOverflow.setup();
  waterLevelStartProgram.setup();
  waterLevelMainBucketLow.setup();

  pinMode(pumpPin, OUTPUT);

  //precisePump.setPin(precisePumpPin);
  //precisePump.getFlowRateAndSpeed();

  waterPumpButton.setup();
  changeSolenoidButton.setup();

  waterValve.setup();
  nutrition1Valve.setup();
  nutrition2Valve.setup();
  nutrition3Valve.setup();

  // enable a call to the 'interrupt service handler' (ISR) on every falling edge at the interrupt pin
  // not using flow sensor at this version
  //attachInterrupt(digitalPinToInterrupt(flowSensorPin), pulseCounter, FALLING);
  flowMeter.reset();

  //not using precisePump in this version
  //state[0] = new NutritionState(&nutrition1Valve, &precisePump, 30);
  //state[2] = new NutritionState(&nutrition2Valve, &precisePump, 30);
  //state[4] = new NutritionState(&nutrition3Valve, &precisePump, 30);

  const float grow  = 3.96; //ml per l
  const float micro  = 2.64; // ml per l
  const float bloom = 1.32; // ml per l

  // we need to fill 6 liters
  float litersPerCycle = 6;

  //flush it a bit
  state[0] = new WateringState(&waterValve, pumpPin, &flowMeter, 100);
  
  state[1] = new WateringState(&nutrition1Valve, pumpPin, &flowMeter, litersPerCycle * bloom);  //7.92 ml
  state[2] = new WateringState(&waterValve, pumpPin, &flowMeter, 900);

  state[3] = new WateringState(&nutrition2Valve, pumpPin, &flowMeter, litersPerCycle * micro);  //15.84 ml
  state[4] = new WateringState(&waterValve, pumpPin, &flowMeter, 2000);

  state[5] = new WateringState(&nutrition3Valve, pumpPin, &flowMeter, litersPerCycle * grow); //23.76 ml
  state[6] = new WateringState(&waterValve, pumpPin, &flowMeter, 3000);
}

void MainWater::closeAllValves()
{
  delay(1000);
  if (waterValve.isOpen())
  {
    waterValve.close();
    delay(1000);
  }

  if (nutrition1Valve.isOpen())
  {
    nutrition1Valve.close();
    delay(1000);
  }

  if (nutrition2Valve.isOpen())
  {
    nutrition2Valve.close();
    delay(1000);
  }

  if (nutrition3Valve.isOpen())
  {
    nutrition3Valve.close();
    delay(1000);
  }
}

void MainWater::loop()
{
  int newState = currentState;

  bool waterOnFloor = waterOnFloorDetector.isWaterOnFloor();
  bool highWater = waterLevelOverflow.isInWater();
  bool lowMainWater = !waterLevelMainBucketLow.isInWater();

  if (waterOnFloor || highWater)
  {
    //This is emergency, the water has been detected on the floor.
    //Shut down everything.
    mainSensors.getLCD()->home();
    mainSensors.getLCD()->clear();
    mainSensors.getLCD()->setCursor(0, 0);

    if (waterOnFloor)
    {
      String alert = "WATER  ON THE FLOOR DETECTED!";
      mainSensors.getLCD()->print(alert);
      Serial.println(alert);
    }

    if (highWater)
    {
      String alert = "WATER OVERFLOW!";
      mainSensors.getLCD()->print(alert);
      Serial.println(alert);
    }

    delay(1000);
    newState = STATE_IDLE;
  }

  if(lowMainWater)
  {
    mainSensors.getLCD()->home();
    mainSensors.getLCD()->clear();
    mainSensors.getLCD()->setCursor(0, 0);

    String alert = "Main water low";
    mainSensors.getLCD()->print(alert);
    Serial.println(alert);

    delay(1000);
    newState = STATE_IDLE;
  }

  if (currentState == STATE_IDLE)
  {
    mainSensors.sendSensorsData(waterOnFloor, highWater, lowMainWater);
    
    if (waterOnFloor || highWater)
      return;

    // water level in the bucket with the flower is low, start the cycle.
    if(!waterLevelStartProgram.isInWater())
    {
      Serial.println("STARTING WATERING PROGRAM");
      newState = 0;
    }

    // manual start
    if (waterPumpButton.isDown())
    {
      Serial.println("Start cycle");
      newState = 0;
    }
    
    // manual solenoid opener (to pump the water through initially when the pipes are with air)
    if (changeSolenoidButton.isDown())
    {
      ++activeOpenSolenoid;
      if (activeOpenSolenoid >= 5)
        activeOpenSolenoid = 0;

      closeAllValves();
      switch (activeOpenSolenoid)
      {
        case 1:
          waterValve.open();
          break;
        case 2:
          nutrition1Valve.open();
          break;
        case 3:
          nutrition2Valve.open();
          break;
        case 4:
          nutrition3Valve.open();
          break;
      }
    }
  }

  if (currentState >= 0)
  {
    state[currentState]->lifeCycle();
    if (state[currentState]->canChangeToNextState())
    {
      Serial.println("Changing state");
      newState++;
    }

    if (newState == MAX_STATE)
      newState = STATE_IDLE;
  }

  if (newState == currentState)
    return; //no changes in system

  Serial.println("A new state is: ");
  Serial.println(newState);

  //a change in the system state. close all valves and stop all pumps
  closeAllValves();
  Serial.println("STOP");
  digitalWrite(pumpPin, LOW);
  delay(1000);
  //precisePump.stop();

  delay(1000);

  if (newState != STATE_IDLE)
  {
    state[newState]->startState();
  }

  currentState = newState;
}
