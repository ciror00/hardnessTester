#include "Visualizer.h"

void Visualizer::begin(const uint8_t address, const uint8_t sda, const uint8_t scl){
  this->address;
  this->sda;
  this->scl;
  this->display(address, sda, scl);
}

void Visualizar::showMessage(const String text){
  this->display.clear();
  this->display.setTextAlignment(TEXT_ALIGN_CENTER);
  this->display.setFont(ArialMT_Plain_16);
  this->display.drawString(0, 0, text);
  this->display.display();
}

void Visualizer::showMeassure(String value, const String unit){
  this->display.clear();
  this->display.setTextAlignment(TEXT_ALIGN_CENTER);
  this->display.setFont(ArialMT_Plain_16);
  this->display.drawString(0, 0, value);
  this->display.setFont(ArialMT_Plain_16);
  this->display.drawString(0, 30, unit);
  this->display.display();
}

void Visualizer::showImage(Images img){
  this->display.clear();
  switch (img) {
    case LOGO: this->display.drawXbm(0,5,logo_width,logo_height,(const unsigned char *)logo);
    case PUSH: this->display.drawXbm(0,5,push_width,push_height,(const unsigned char *)push);
  }
  this->display.display();
}

void Visualizer::disclaimer(){
  this->display.clear();
  this->display.display();
}
