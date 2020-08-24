#ifndef __LITEVISUALIZER__h__
#define __LITEVISUALIZER__h__
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//#include "Symbol.h"

class LiteVisualizer{
  LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);
  
  public:
    LiteVisualizer() = default;
    void begin();
    bool switcher(bool n);
    void showSettings();
    void showMessage(String text, String header = " ", String footer = " ", bool clear = false);
    void showMeasure(int line, String value, String unit = "[Kg]", String footer = " ");
    void reset();
    ~LiteVisualizer() = default;
};
#endif
