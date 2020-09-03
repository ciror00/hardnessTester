#ifndef __LITEVISUALIZER__h__
#define __LITEVISUALIZER__h__
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "Symbol.h"

class LiteVisualizer{
  LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4); // columna || fila
  void _showSettings(bool _sd_status, bool _gps_status);

  public:
    LiteVisualizer() = default;
    void begin();
    bool switcher(bool n);
    void showMessage( String header, String text, String footer, bool clear = true);
    void showMeasure(int line, String label, String value, String unit, bool clear = true);
    void showHeader(bool sd_status, bool gps_status, bool clear = true);
    void home(bool sd_status, bool gps_status);
    void detail(bool sd_status, bool gps_status, String number);
    void summary(String averange, String max, String distance, String regiter);
    void reset();
    ~LiteVisualizer() = default;
};
#endif
