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

  int spear;
  int dig = 0;

  int _getSize(int iteration = 5);

	public:
		TapeMeasure() = default;
    bool begin();
    int calibrateLance(int iteration = 5);
    int takeSize(int iteration = 5);
    void reset();
		~TapeMeasure() = default;
};
#endif
