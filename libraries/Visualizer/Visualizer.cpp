#include "Visualizer.h"

void Visualizer::begin(){
  //this->oled = SSD1306Wire(0x3c, 0, 14);
  this->oled.init();
  this->oled.flipScreenVertically();
}

void Visualizer::showMessage(String text){
  this->oled.clear();
  this->oled.setTextAlignment(TEXT_ALIGN_CENTER);
  this->oled.setFont(ArialMT_Plain_16);
  this->oled.drawString(0, 0, text);
  this->oled.display();
}

void Visualizer::showMeasure(String value, String unit){
  this->oled.clear();
  this->oled.setTextAlignment(TEXT_ALIGN_CENTER);
  this->oled.setFont(ArialMT_Plain_16);
  this->oled.drawString(0, 0, value);
  this->oled.setFont(ArialMT_Plain_10);
  this->oled.drawString(0, 30, unit);
  this->oled.display();
}

void Visualizer::showImage(Images img){
  this->oled.clear();
  switch (img) {
    case LOGO: this->oled.drawXbm(0,5,logo_width,logo_height,(const unsigned char *)logo_ZYX);
    case PUSH: this->oled.drawXbm(0,5,push_width,push_height,(const unsigned char *)push);
  }
  this->oled.display();
}

void Visualizer::disclaimer(){
  this->oled.clear();
  this->oled.display();
}
