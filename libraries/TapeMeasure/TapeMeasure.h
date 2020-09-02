#ifndef __TapeMeasure__h__
#define __TapeMeasure__h__
//#define PRINT(...) Serial.print(__VA_ARGS__)

#include <Arduino.h>
#include <NewPing.h>

class TapeMeasure{
  const int limit = 100;
  const int trig = 8;
  const int echo = 9;

  NewPing sonar = NewPing(trig, echo, 100);

  int spear = 0;
  int dig = 0;

  unsigned int _getSize(int iteration = 5);

	public:
		TapeMeasure() = default;
    bool begin();
    int calibrateLance(int iteration = 5);
    int makeAWell(int iteration = 5, int tolerance = 0);
    void reset();
		~TapeMeasure() = default;
};
#endif
