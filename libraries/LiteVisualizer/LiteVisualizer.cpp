#include "LiteVisualizer.h"

void LiteVisualizer::begin(){
  this->lcd.init();
}

bool LiteVisualizer::switcher(bool n){
  if(n==1){
    this->lcd.backlight();
    return true;
  }else{
    this->lcd.noBacklight();
    return false;
  }
}

void LiteVisualizer::showSettings(){
  ;
}

void LiteVisualizer::showMessage(String text, String header, String footer, bool clear){
  if(clear)this->lcd.clear();
  this->lcd.setCursor(1, 1);
  this->lcd.print(header);
  this->lcd.setCursor(2, 1);
  this->lcd.print(text);
  this->lcd.setCursor(3, 1);
  this->lcd.print(footer);
}

void LiteVisualizer::showMeasure(int line, String value, String unit, String footer){
  this->lcd.clear();
  if(line == 1 || line == 2){
    this->lcd.setCursor(line, 1);
    this->lcd.print(value);
    this->lcd.setCursor(line, 10);
    this->lcd.print(unit);
  }
  this->lcd.setCursor(3, 10);
    this->lcd.print(footer);
}

void LiteVisualizer::reset(){
  this->lcd.clear();
}
