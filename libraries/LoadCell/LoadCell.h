#ifndef __LOADCELL__h__
#define __LOADCELL__h__
#include <Arduino.h>
  #ifdef ESP_PORT
  #define MSG(...) ESP_PORT.printf( __VA_ARGS__ )
  #else
  #define MSG(...)
  #endif
#include <math.h>
#include <HX711.h>


// Documentacion https://github.com/bogde/HX711
class LoadCell{
  HX711 doubleEnded;
  long layover;
  long currentValue;
  long readValue = 0;
  long t = 0;
  public:
    LoadCell() = default;
    void begin(const int dout, const int pd_sck, const byte gain = 128);
    long strength();
    long strengthAverage(int amount);
    long raw();
    void calibrate(long patternWeight, int samples = 20, byte iteration = 1);
    ~LoadCell() = default;
};
#endif
