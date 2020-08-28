#include "LiteVisualizer.h"

void LiteVisualizer::begin(){
  this->lcd.init();
  this->lcd.createChar(0, global);
  this->lcd.createChar(1, card_ok);
  this->lcd.createChar(2, error);
  this->lcd.createChar(3, arrow);
}

void LiteVisualizer::_showSettings(bool _sd_status, bool _gps_status){
  byte file = 0;
  this->lcd.clear();
  // Mensaje de GPS por apntalla
  this->lcd.setCursor(0, file);
  if(_sd_status){
    this->lcd.write(0);
    this->lcd.scrollDisplayRight();
    this->lcd.print("GPS.Ok");
  }else{
    this->lcd.write(0);
    this->lcd.scrollDisplayRight();
    this->lcd.print("NO.GPS");
  }
  this->lcd.setCursor(8, file);
  // Mensaje de SD por pantalla
  if(_gps_status){
    this->lcd.write(1);
    this->lcd.scrollDisplayRight();
    this->lcd.print("SD.OK");
  }else{
    this->lcd.write(1);
    this->lcd.scrollDisplayRight();
    this->lcd.print("NO.SD");
  }
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

void LiteVisualizer::showMessage(String header, String text, String footer, bool clear){
  if(clear)this->lcd.clear();
  this->lcd.setCursor(0, 1);
  this->lcd.print(header);
  this->lcd.setCursor(0, 2);
  this->lcd.print(text);
  this->lcd.setCursor(0, 3);
  this->lcd.print(footer);
}

void LiteVisualizer::showMeasure(int line, String label, String value, String unit, bool clear){
  if(clear)this->lcd.clear();
  if(line == 2 || line == 3){
    this->lcd.setCursor(1, line);
  }
  this->lcd.print(label);
  this->lcd.scrollDisplayRight();
  this->lcd.print(value);
  this->lcd.setCursor(10, line);
  this->lcd.print(unit);
}

void LiteVisualizer::reset(){
  this->lcd.clear();
}

void LiteVisualizer::home(bool sd_status, bool gps_status){
  this->_showSettings(sd_status, gps_status);
  this->lcd.setCursor(1, 3);
  this->lcd.write(3);
  this->lcd.setCursor(18, 3);
  this->lcd.write(3);
  this->showMessage(" ", "Listo para Medir", " ", false);
}

void LiteVisualizer::detail(bool sd_status, bool gps_status, String number){
  //char buff[10];
  this->lcd.clear();
  this->_showSettings(sd_status, gps_status);
  this->lcd.setCursor(0, 1);
  this->lcd.print("Guardado SD N°");
  this->lcd.setCursor(14, 1);
  if(sd_status){
    this->lcd.print(number);
  }else{
    this->lcd.print(number);
  }
}

void LiteVisualizer::summary(String max, String averange, String distance, String regiter){
  // Lado izquierdo
  this->lcd.setCursor(0, 2);
  this->lcd.print(averange);
  this->lcd.setCursor(4, 2);
  this->lcd.print("Kg Pmax:");
  this->lcd.print(max);
  this->lcd.setCursor(18, 2);
  this->lcd.print("Kg");
  // Lado derecho
  this->lcd.setCursor(0, 3);
  this->lcd.print(distance);
  this->lcd.setCursor(4, 3);
  this->lcd.print("cm Dmax:");
  this->lcd.print(regiter);
  this->lcd.setCursor(18, 3);
  this->lcd.print("cm");
}