#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

class State
{
public:
  State()
  {}

  virtual void startState() = 0;
  virtual void lifeCycle() = 0;
  virtual bool canChangeToNextState() = 0;
};

#endif // STATE_H_INCLUDED
