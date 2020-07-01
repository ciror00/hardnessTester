#include "hardnessTester.h"

Visualizer display;
LoadCell measure;
Recorder recorder;

bool conf;
byte button = 0;
int sensibility = 100;
long strength, average;

char strength_buff[50];
char average_buff[50];

void setup(){
  Serial.begin(115200);
  Serial.println("\nProyecto \"hardnessTester\"\n");
  display.begin();
  measure.begin(DT_CELL, SCK_CELL);
  display.showImage(LOGO);
  conf = waitForUser(5000);
  if(conf)measure.calibrate(3, 10, 10);
  Serial.println();
  //Serial.print("Crudo: ");
  //Serial.println(measure.raw());
  pinMode(TOUCH, INPUT); // 0: No pulsado | 1: Pulsado
  if(recorder.begin(CS)){
    Serial.println("SD y Reloj funcionando");
    // Se agregan los titulos de archivos, que viene despues de los "Fecha" y "Hora" (titulos por defecto)
    recorder.setTitles(7, "Lote", "Frutas total", "Fruta", "Dureza", "Procentaje", "Promedio", "Dureza maxima");
  }else{
    Serial.println("Error en SD y/o Reloj");
  }
  waitForMachine(1000);
}

void loop(){
  button = digitalRead(TOUCH);
  if(button == 1){
    Serial.println("\nListo para medir");
    if(measure.raw() > sensibility){
      strength = measure.strength();
      average = measure.strengthAverage(5);
      display.showMeasure((String)average, "kgf");
      sprintf(strength_buff, "%ld", strength); sprintf(average_buff, "%ld", average);
      recorder.saveRegistry(7, "X", "Y", "Z", strength_buff, "%", average_buff, "Maximo");
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
