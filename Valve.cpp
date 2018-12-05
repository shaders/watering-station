#include <Arduino.h>
#include "Valve.h"

Valve::Valve(int pin)
: pin(pin),
  state(CLOSED)
{}

void Valve::setup()
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);

  Serial.print("Setup - close valve: ");
  Serial.println(pin);
}

bool Valve::isOpen()const
{
  return state == OPEN;
}

void Valve::close()
{
  Serial.print("Closing valve: ");
  Serial.println(pin);
  digitalWrite(pin, LOW);
  state = CLOSED;
}

void Valve::open()
{
  Serial.print("Open valve: ");
  Serial.println(pin);
  
  digitalWrite(pin, HIGH);
  state = OPEN;
}
