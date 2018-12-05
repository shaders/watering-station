#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

class Button
{
  int pin;

public:
  Button(int pin)
  : pin(pin)
  {}

  void setup()
  {
    pinMode(pin, INPUT_PULLUP);
  }

  bool isDown()
  {
    int state = digitalRead(pin);
    if(state == LOW)
      return true;
    
    return false;
  }
};

#endif   // BUTTON_H_INCLUDED
