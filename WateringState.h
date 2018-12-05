#ifndef WATERING_STATE_H_INCLUDED
#define WATERING_STATE_H_INCLUDED

#include "State.h"

class Valve;
class FlowMeter;

class WateringState : public State
{
  unsigned long waterToFillMl;
  unsigned long timeToRun;
  
  unsigned long lastTime;
  unsigned long currentRunningTime;
  long tickTime;

  int pumpPin;
  Valve * valve;
  FlowMeter * flowMeter;

  void startWaterPump();
  void stopWaterPump();

public:
  WateringState(Valve * valve, int pumpPin, FlowMeter * flowMeter, unsigned long waterToFillMl);

  virtual void startState();
  virtual void lifeCycle();
  virtual bool canChangeToNextState();
};

#endif // WATERING_STATE_H_INCLUDED
