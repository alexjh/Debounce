#ifndef Debounce_h
#define Debounce_h

#include <inttypes.h>

/* 
  There are two debouncing strategies to choose from: 
  Lockout and Stable.  See below.
*/

class Debounce
{
public:
  // Create an instance of the bounce library
  Debounce(); 
  // Attach to a pin (and also sets initial state)
  virtual void attach(int pin);
	// Sets the debounce interval
  void interval(unsigned long interval_millis); 
	// Updates the pin
	// Returns 1 if the state changed
	// Returns 0 if the state did not change
  virtual bool update() = 0; 
	// Returns the updated pin state
  uint8_t read();
  
protected:
  unsigned long previous_millis;
  unsigned long interval_millis;
  uint8_t debouncedState;
  uint8_t unstableState;
  uint8_t pin;
  uint8_t stateChanged;
};

/* 
  The Lockout strategy fires an update immediately when the
  pin changes, then waits for the specified interval before firing
  another update.  This eliminates any delay between the physical
  action and the response but can be triggered by transient noise
  and can be falsely re-triggered by bounce noise if it lasts longer
  than the interval period. 
*/

class LockoutDebounce : public Debounce
{
public:
  virtual void attach(int pin);
  virtual bool update(); 
};

/* 
  The Stable strategy does not fire immediately when the pin changes
  but rather waits for the pin to remain stable at the new state for
  an entire interval period before firing an update.  This avoids being
  triggered by transient noise and can handle noise of any duration but
  introduces a delay between the physical action and the response.
*/

class StableDebounce : public Debounce
{
public:
  virtual void attach(int pin);
  virtual bool update(); 
};

#endif
