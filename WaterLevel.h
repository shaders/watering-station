#ifndef WATER_LEVEL_H_INCLUDED
#define WATER_LEVEL_H_INCLUDED

class WaterLevel
{
  int pin;

public:
  WaterLevel(int pin)
  : pin(pin)
  {}

  void setup()
  {
    pinMode(pin, INPUT_PULLUP);
  }

  bool isInWater()
  {
    int state = digitalRead(pin);
    if(state == HIGH)
      return true;
    
    return false;
  }
};

#endif // WATER_LEVEL_H_INCLUDED
