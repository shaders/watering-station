#ifndef NUTRITION_STATE_H_INCLUDED
#define NUTRITION_STATE_H_INCLUDED

#include "State.h"

class GravityPump;
class Valve;

class NutritionState : public State
{
  unsigned long nutritionToFillMl;

  GravityPump * precisePump;
  Valve * valve;

public:
  NutritionState(Valve * valve, GravityPump * precisePump, unsigned long nutritionToFillMl);

  virtual void startState();
  virtual void lifeCycle();  
  virtual bool canChangeToNextState();
};

#endif // NUTRITION_STATE_H_INCLUDED
