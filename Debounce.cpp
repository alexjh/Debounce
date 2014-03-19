#include "Arduino.h"
#include "Debounce.h"

extern "C" void __cxa_pure_virtual() { while (1); }

Debounce::Debounce()
{
  this->interval_millis = 10;
}

void Debounce::attach(int pin)
{
 this->pin = pin;
 debouncedState = unstableState = digitalRead(pin);
}

void Debounce::interval(unsigned long interval_millis)
{
  this->interval_millis = interval_millis; 
}

uint8_t Debounce::read()
{
  return debouncedState;
}

void LockoutDebounce::attach(int pin)
{
  Debounce::attach(pin);
  previous_millis = 0;
}

bool LockoutDebounce::update()
{
  stateChanged = false;
  // Ignore everything if we are locked out
  if (millis() - previous_millis >= interval_millis)
  {
    uint8_t currentState = digitalRead(pin);
    if (debouncedState != currentState )
    {
      previous_millis = millis();
      debouncedState = currentState;
      stateChanged = true;
    }
  }
  return stateChanged;
}

void StableDebounce::attach(int pin)
{
  Debounce::attach(pin);
  previous_millis = millis();
}

bool StableDebounce::update()
{
  uint8_t currentState = digitalRead(pin);
  stateChanged = false;

  if (currentState != unstableState)
  {
    previous_millis = millis();
  }
  else if (millis() - previous_millis >= interval_millis && currentState != debouncedState)
  {
    debouncedState = currentState;
    stateChanged = true;
  }
   
  unstableState = currentState;
  return stateChanged;
}

void ThresholdDebounce::attach(int pin)
{
  Debounce::attach(pin);
  previous_millis = millis();
}

bool ThresholdDebounce::update()
{
  uint8_t currentState = digitalRead(pin);

  numberOfSamples++;
  if (currentState == HIGH)
  {
    highSamples++;
  }

  stateChanged = false;

  if (millis() - previous_millis >= interval_millis && currentState != debouncedState)
  {
    float percentHigh = highSamples / numberOfSamples;
    uint8_t intervalState = (percentHigh > thresholdValue);
    if (intervalState != debouncedState)
    {
      debouncedState = intervalState;
      stateChanged = true;
    }
    numberOfSamples = 0;
    highSamples = 0;
  }

  return stateChanged;
}

void ThresholdDebounce::threshold(float value)
{
  thresholdValue = value;
}
