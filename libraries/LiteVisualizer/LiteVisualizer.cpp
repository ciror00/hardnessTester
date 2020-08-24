#include "LiteVisualizer.h"

void LiteVisualizer::begin(){
  this->lcd.init();
}

bool LiteVisualizer::switcher(bool n){
  if(n==true){
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
  this->lcd.setCursor(1, 2);
  this->lcd.print(text);
  this->lcd.setCursor(1, 3);
  this->lcd.print(footer);
}

void LiteVisualizer::showMeasure(int line, String value, String unit, String footer){
  this->lcd.clear();
  if(line == 1 || line == 2){
    this->lcd.setCursor(1, line);
    this->lcd.print(value);
    this->lcd.setCursor(10, line);
    this->lcd.print(unit);
  }
  this->lcd.setCursor(10, 3);
    this->lcd.print(footer);
}

void LiteVisualizer::reset(){
  this->lcd.clear();
}
