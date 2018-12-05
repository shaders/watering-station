#include <Arduino.h>

#include "NutritionState.h"
#include "Valve.h"
#include "libs/GravityPump/GravityPump.h"

NutritionState::NutritionState(Valve * valve, GravityPump * precisePump, unsigned long nutritionToFillMl)
: valve(valve),
  precisePump(precisePump),
  nutritionToFillMl(nutritionToFillMl)
{
    Serial.println("NUTRITION");
}

void NutritionState::startState()
{
  delay(1000);
  valve->open();
  delay(1000);
  
  //pump some ml of nutrution
  precisePump->calFlowRate(0);
  float result = precisePump->flowPump(nutritionToFillMl);

  Serial.print("Start nutrition state. Will run for (sec): ");
  Serial.println(result/1000.0);
}

void NutritionState::lifeCycle()
{
  precisePump->update();
  precisePump->calFlowRate(0);
}

bool NutritionState::canChangeToNextState()
{
  bool isRunning = precisePump->isRunning();
  if(precisePump->isRunning() == false)
  {
    Serial.println("NUTE: changing state");
    delay(1000);
    valve->close();
    delay(1000);
    return true;
  }

  return false;
}
