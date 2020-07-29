#include "Visualizer.h"

void Visualizer::begin(){
  this->oled.init();
  this->oled.flipScreenVertically();
}

void Visualizer::showMessage(String text){
  this->oled.clear();
  this->oled.setTextAlignment(TEXT_ALIGN_CENTER);
  this->oled.setFont(ArialMT_Plain_16);
  this->oled.drawString(64, 30, text);
  this->oled.display();
}

void Visualizer::showMeasure(String value, String unit, bool refresh){
  if(refresh)this->oled.clear();
  this->oled.setTextAlignment(TEXT_ALIGN_CENTER);
  this->oled.setFont(ArialMT_Plain_24);
  this->oled.drawString(64, 15, value);
  this->oled.setFont(ArialMT_Plain_16);
  this->oled.drawString(64, 45, unit);
  this->oled.display();
}

void Visualizer::showImage(Images img){
  this->oled.clear();
  switch (img) {
    case ZYX: this->oled.drawXbm(32,5,zyx_width,zyx_height,(const unsigned char *)logo_ZYX);break;
    case COP: this->oled.drawXbm(32,5,cop_width,cop_height,(const unsigned char *)logo_Copain);break;
    case PUSH: this->oled.drawXbm(32,5,push_width,push_height,(const unsigned char *)push);break;
    case CARD: this->oled.drawXbm(50,10,sd_width,sd_height,(const unsigned char *)sdCard);break;
    case NOCARD: this->oled.drawXbm(50,10,sd_width,sd_height,(const unsigned char *)no_sdCard);break;
    case TOOL: this->oled.drawXbm(40,10,tools_width,tools_height,(const unsigned char *)tools);break;
  }
  this->oled.display();
}

void Visualizer::reset(){
  this->oled.clear();
  this->oled.display();
}
