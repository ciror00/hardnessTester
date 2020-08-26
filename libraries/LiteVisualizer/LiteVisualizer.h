#ifndef __LITEVISUALIZER__h__
#define __LITEVISUALIZER__h__
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "Symbol.h"

class LiteVisualizer{
  LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4); // columna || fila

  public:
    LiteVisualizer() = default;
    void begin();
    bool switcher(bool n);
    void showSettings(bool sd_status, String status);
    void showMessage(String text, String header = " ", String footer = " ", bool clear = true);
    void showMeasure(int line, String label, String value, String unit, bool clear = true);
    void reset();
    ~LiteVisualizer() = default;
};
#endif
