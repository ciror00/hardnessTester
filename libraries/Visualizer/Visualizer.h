#ifndef __VISUALIZER__h__
#define __VISUALIZER__h__
#include <Arduino.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include <SPI.h>
#include "Images.h"

enum Images {LOGO,PUSH};

class Visualizer{
  SSD1306Wire oled = SSD1306Wire(0x3c, 0, 14);
  public:
    Visualizer() = default;
    void begin();
    void showMessage(String text);
    void showMeasure(String value, String unit, bool refresh = true);
    void showImage(Images img);
    void reset();
    ~Visualizer() = default;
};
#endif
