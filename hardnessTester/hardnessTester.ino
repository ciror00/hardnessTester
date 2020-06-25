#include "hardnessTester.h"

Visualizer display;
LoadCell measure;

bool conf;
byte button = 0;
int sensibility = 100;
long strength, average;

void setup(){
  Serial.begin(115200);
  Serial.println("\nProyecto \"hardnessTester\"\n");
  display.begin();
  measure.begin(DOUT, SCK);
  display.showImage(LOGO);
  conf = waitForUser(5000);
  if(conf)measure.calibrate(3, 10, 10);
  //Serial.print("Crudo: ");
  //Serial.println(measure.raw());
  pinMode(TOUCH, INPUT); // 0: No pulsado | 1: Pulsado
  waitForMachine(1000);
}

void loop(){
  button = digitalRead(TOUCH);
  if(button == 1){
    Serial.println("Boton presionado");
    if(measure.raw() > sensibility){
      strength = measure.strength();
      average = measure.strengthAverage(5);
      display.showMeasure((String)average, "kgf");
      Serial.print("Fuerza: "); Serial.println(strength);
      Serial.print("Fuerza promedio: "); Serial.println(average);
    }else{
      display.showMessage("Listo para medir");
    }
  }else{
    display.showImage(PUSH);
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

void waitForMachine(int period){
  unsigned long time_now = millis();
  while(millis() < time_now + period);
}
