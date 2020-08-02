#ifndef __VISUALIZER__h__
#define __VISUALIZER__h__
#include <Arduino.h>
#include <Wire.h>
#include "SH1106Wire.h" //#include "SSD1306Wire.h"
#include <SPI.h>
#include "Images.h"

enum Images {ZYX,COP,PUSH, CARD, NOCARD, TOOL, ARROW};

class Visualizer{
  // Se agregan pines fijos por defecto 5 (D1) y 4 (D2).
  //SSD1306Wire oled = SSD1306Wire(0x3c, 5, 4);
  SH1106Wire oled = SH1106Wire(0x3c, 5, 4);
  public:
    Visualizer() = default;
    void begin();
    void showMessage(String text, String header = " ", String footer = " ", bool clear = true);
    void showMeasure(String value, String subtitle = " ", String footer = " ");
    void showImage(Images img, String footer = " ");
    void reset();
    ~Visualizer() = default;
};
#endif
