#ifndef __VISUALIZER__h__
#define __VISUALIZER__h__
#include <Arduino.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include "Images.h"

enum Images {LOGO,PUSH};

class Visualizer{
  //SSD1306Wire oled;
  SSD1306Wire oled = SSD1306Wire(0xc3, 0, 14);
  public:
    Visualizer() = default;
    void begin(const byte address, const byte sda, const byte scl);
    void showMessage(const String text);
    void showMeassure(String value, const String unit);
    void showImage(Images img);
    void disclaimer();
    ~Visualizer() = default;
};
#endif
