#include "LiteVisualizer.h"

void LiteVisualizer::begin(){
  this->lcd.init();
  this->lcd.createChar(0, global);
  this->lcd.createChar(1, card_ok);
  this->lcd.createChar(2, error);
  this->lcd.createChar(3, arrow);
  this->lcd.createChar(4, clean);
}

void LiteVisualizer::_showSettings(bool _sd_status, bool _gps_status){
  byte file = 0;
  this->lcd.clear();
  // Mensaje de GPS por apntalla
  this->lcd.setCursor(0, file);
  if(_gps_status){
    this->lcd.write(0);
    //this->lcd.scrollDisplayRight();
    this->lcd.print("GPS.Ok");
  }else{
    this->lcd.write(0);
    //this->lcd.scrollDisplayRight();
    this->lcd.print("NO.GPS");
  }
  this->lcd.setCursor(8, file);
  // Mensaje de SD por pantalla
  if(_sd_status){
    this->lcd.write(1);
    //this->lcd.scrollDisplayRight();
    this->lcd.print("SD.OK");
  }else{
    this->lcd.write(1);
    //this->lcd.scrollDisplayRight();
    this->lcd.print("NO.SD");
  }
}

void LiteVisualizer::showHeader(bool sd_status, bool gps_status){
  this->lcd.setCursor(0, 0);
  this->lcd.print("                   ");
  this->_showSettings(sd_status, gps_status);
  //byte file = 0;
  // Mensaje de GPS por apntalla
  //this->lcd.setCursor(0, file);
  //this->lcd.print("                   ");
  //this->lcd.setCursor(0, file);
  //if(gps_status){
  //  this->lcd.write(0);
  //  this->lcd.print("GPS.Ok");
  //}else{
  //  this->lcd.write(0);
  //  this->lcd.print("NO.GPS");
  //}
  //this->lcd.setCursor(8, file);
  // Mensaje de SD por pantalla
  //if(sd_status){
  //  this->lcd.write(1);
  //  this->lcd.print("SD.OK");
  //}else{
  //  this->lcd.write(1);
  //  this->lcd.print("NO.SD");
  //}
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

void LiteVisualizer::showMeasure(int line, String value, String label, String unit, bool clear){
  if(clear)this->lcd.clear();
  if(line == 1 || line == 2){
    this->lcd.setCursor(1, line);
  }
  this->lcd.print(value);
  this->lcd.print(label);
  this->lcd.setCursor(10, line);
  this->lcd.print(unit);
}

void LiteVisualizer::reset(){
  this->lcd.clear();
}

void LiteVisualizer::home(bool sd_status, bool gps_status){
  byte file = 3;
  this->_showSettings(sd_status, gps_status);
  this->showMessage(" ", "Listo para Medir", " ", false);
  this->lcd.setCursor(1, file);
  this->lcd.write(3);
  this->lcd.setCursor(14, file);
  this->lcd.write(3);
}

void LiteVisualizer::detail(bool sd_status, bool gps_status, String number){
  byte file = 1;
  this->lcd.clear();
  this->_showSettings(sd_status, gps_status);
  this->lcd.setCursor(0, file);
  if(sd_status){
    this->lcd.print("Guardado OK.No:");
    this->lcd.setCursor(14, file);
    this->lcd.print(number);
  }else{
    this->lcd.print("No guardado.No:");
    this->lcd.setCursor(14, file);
    this->lcd.print(number);
  }
}

void LiteVisualizer::summary(String averange, String max, String distance, String regiter){
  byte column[] = {0, 10};
  byte file[] = {2,3};
  String Fp = "Fp:" + averange;
  String Fm = "Fm:" + max;
  String Dm = "Dm: " + distance;
  String Dfm = "Dfm: " + regiter;
  this->lcd.setCursor(column[0], file[0]);
  this->lcd.print(Fp);
  this->lcd.setCursor(column[1], file[0]);
  this->lcd.print(Fm);
  this->lcd.setCursor(column[0], file[1]);
  this->lcd.print(Dm);
  this->lcd.setCursor(column[1], file[1]);
  this->lcd.print(Dfm);
}
