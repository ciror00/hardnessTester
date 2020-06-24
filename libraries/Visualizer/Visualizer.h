#ifndef __VISUALIZER__h__
#define __VISUALIZER__h__
#include <Arduino.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include <SPI.h>
#include "Images.h"
#define PRINT(...) Serial.print(__VA_ARGS__)

enum Images {LOGO,PUSH};

class Visualizer{
  //SSD1306Wire oled;
  SSD1306Wire oled = SSD1306Wire(0xC3, 0, 14);
  public:
    Visualizer() = default;
    void begin(const byte address, const byte sda, const byte scl);
    void showMessage(String text);
    void showMeasure(String value, String unit);
    void showImage(Images img);
    void disclaimer();
    ~Visualizer() = default;
};
#endif
