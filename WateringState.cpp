#include "WateringState.h"

#include <Arduino.h>
#include "Valve.h"
#include "libs/FlowMeter/FlowMeter.h"

void WateringState::startWaterPump()
{
  delay(1000);
  Serial.println("Start water pump");
  valve->open();
  delay(1000);

  digitalWrite(pumpPin, HIGH);
}

void WateringState::stopWaterPump()
{
  Serial.println("Stop water pump");
  digitalWrite(pumpPin, LOW);
  delay(1000);

  valve->close();
  delay(1000);
}

WateringState::WateringState(Valve * valve, int pumpPin, FlowMeter * flowMeter, unsigned long waterToFillMl)
: lastTime(0),
  valve(valve),
  pumpPin(pumpPin),
  flowMeter(flowMeter),
  currentRunningTime(0),
  tickTime(0),
  waterToFillMl(waterToFillMl)
{
  timeToRun = waterToFillMl*1000/4.347826087; //this is 500 ml in 115 sec (according to test)
  
  Serial.print("WATER. Fill: ");
  Serial.print(waterToFillMl);
  Serial.print(" (ml), Time to run: ");
  Serial.print(timeToRun/1000.0);
  Serial.println(" (sec)");
}

void WateringState::startState()
{
  Serial.print("Start state WATER. Fill: ");
  Serial.print(waterToFillMl);
  Serial.print(" (ml), Time to run: ");
  Serial.print(timeToRun/1000.0);
  Serial.println(" (sec)");

  startWaterPump();

  flowMeter->reset();
  
  //record the time
  lastTime = millis();
  currentRunningTime = 0;
  tickTime = 0;
}

void WateringState::lifeCycle()
{
  // do some timekeeping
  if(millis() < lastTime)
  {
    //millis overflows according to docs every 50 days
    Serial.println("WATER: MILLIS OVERFLOW");
    lastTime = millis();
  }
  
  unsigned long currentTime = millis();
  unsigned long duration = currentTime - lastTime;
  lastTime = currentTime;

  //just safety protection. No loop should be more that 1 sec delay.
  if(duration >= 1000)
  {
    Serial.println("WATER: Error. Hit a loop longer than 1 sec");
    duration = 1000;
  }

  currentRunningTime += duration;
  tickTime += duration;

  if(tickTime < 1000)
    return;

  // process the counted ticks
  flowMeter->tick(tickTime);

//    Serial.print("Water pump tick: ");
//    Serial.println(tickTime);
//
//    Serial.print("Water pump (ml): ");
//    Serial.println(flowMeter->getTotalVolume() * 1000);

  tickTime = 0;

  // output some measurement result
//    Serial.println("FlowMeter - current flow rate: " + String(flowMeter->getCurrentFlowrate() * 1000) + " ml/min, " +
//                   "nominal volume: " + String(flowMeter->getTotalVolume() * 1000) + " ml, " +
//                   "compensated error: " + String(flowMeter->getCurrentError()) + " %, " +
//                   "duration: " + String(flowMeter->getTotalDuration() / 1000.0) + " s.");    
}

bool WateringState::canChangeToNextState()
{
  float totalMils = flowMeter->getTotalVolume() * 1000;
  if(currentRunningTime >= timeToRun)// || totalMils >= waterToFillMl)
  {
    Serial.print("WATER. Changing state. Filled (ml): ");
    Serial.println(totalMils);

    Serial.print("Time remaining: ");
    long deltaTime = timeToRun - currentRunningTime;
    Serial.println(deltaTime);
    
    stopWaterPump();
    return true;
  }

  return false;
}
