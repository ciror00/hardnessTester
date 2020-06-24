#include "hardnessTester.h"

LoadCell measure;

bool conf;
byte button = 0;

void setup(){
  Serial.begin(115200);
  Serial.println("\nInicio.");
  measure.begin(DOUT, SCK);
  bool conf = waitForUser(5000);
  if(conf)measure.calibrate(3, 10, 10);
  //Serial.print("Crudo: ");
  //Serial.println(measure.raw());
  pinMode(TOUCH, INPUT); // 0: No pulsado | 1: Pulsado
}

void loop(){
  button = digitalRead(TOUCH);
  if(measure.raw() && button == 1){
    Serial.print("Fuerza: ");
    Serial.println(measure.strength());
    Serial.print("Fuerza promedio: ");
    Serial.println(measure.strengthAverage(10));
  }
}

bool waitForUser(int period){
  byte in;
  unsigned long time_now = millis();
  Serial.print("¿Quiere realiza la calibracion?");
  while(millis() < time_now + period){
    if (Serial.available() > 0) {
      in = Serial.read();
      break;
    }
  }
  if(in == 121 || in == 89){
    return true;
  }else{
    return false;
  }
}
