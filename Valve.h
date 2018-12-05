#ifndef VALVE_H_INCLUDED
#define VALVE_H_INCLUDED

class Valve
{
public:
  enum State {CLOSED = 0, OPEN};

private:
  int pin;
  State state;

  Valve(const Valve &);
  Valve & operator = (const Valve &);

public:  
  Valve(int pin);

  void setup();

  bool isOpen()const;

  void close();
  void open();
};

#endif // VALVE_H_INCLUDED
