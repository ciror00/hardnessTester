#ifndef __LOADCELL__h__
#define __LOADCELL__h__
#include <Arduino.h>
#include <HX711.h>
#define PRINT(...) Serial.print(__VA_ARGS__)


// Documentacion https://github.com/bogde/HX711
class LoadCell{
  HX711 doubleEnded;

  byte dout, pd_sck, gain;
  long layover;
  long currentValue;
  long crudeValue;
  long readValue = 0;

  public:
    LoadCell() = default;
    void begin(const byte dout, const byte pd_sck, const byte gain = 128);
    long strength();
    long strengthAverage(int amount);
    long raw();
    void calibrate(long patternWeight, int samples = 20, int iteration = 1);
    ~LoadCell() = default;
};
#endif
