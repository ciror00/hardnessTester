#include "hardnessTester.h"

Visualizer display;
LoadCell measure;
Recorder recorder;
DataHandler dataHandler;

void setup(){
  Serial.begin(115200);
  EEPROM.begin(32); // Se reservar 32 Bytes (256 bits) | Tamaño maximo 4K (4096)
  display.begin();
  measure.begin(DT_CELL, SCK_CELL);
  display.showImage(COP);
  Serial.println("\nProyecto: \"Hardness Tester\"\n");
  conf = waitForUser(3000, "[INS]\t¿Ingresar factor manualmente? [Y/N]: ");
  if(conf){
    scale = askTheUser("[INS]\tIngreses factor de escala: ");
    Serial.print("ESCALA: "); Serial.println(scale);
    measure.manualSetup(scale);
    EEPROM.write(0,scale);
    EEPROM.commit();
  }
  Serial.println("\n[MSJ]\tConfiguracion manual cancelada");
  conf = waitForUser(3000, "\n[INS]\t¿Iniciar calibracion? [Y/N]: ");
  if(conf){
    patternWeight = askTheUser("[INS]\tIngrese el peso real del equipo: ");
    Serial.print("PESO REAL: "); Serial.println(patternWeight);
    scale = measure.calibrate(patternWeight); // Se ingresa el peso real del equipo
    Serial.print("ESCALA: "); Serial.println(scale);
    EEPROM.write(0,scale);
    EEPROM.commit();
  }
  Serial.println("\n[MSJ]\tCalibracion manual cancelada");
  Serial.print("ESCALA: ");Serial.println(EEPROM.read(0));
  //EEPROM.end();
  // Serial.println();
  pinMode(TOUCH, INPUT); // 0: No pulsado | 1: Pulsado
  if(recorder.begin(CS)){
    Serial.println("[OK]\tSD & Reloj");
    // Se agregan los titulos de archivos, que viene despues de los "Fecha" y "Hora" (titulos por defecto)
    recorder.setTitles(7, "Lote", "Frutas total", "Fruta", "Dureza", "Procentaje", "Promedio", "Dureza maxima");
  }else{
    Serial.println("[ERROR]\tSD & Reloj");
  }
  waitForMachine(1000);
}

void loop(){
  button = digitalRead(TOUCH);
  if(button == 1){
    Serial.println("[MJS]\tListo para medir");
    if(minimumForce(sensibility)){
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

/*
  Se encapsulan funciones para mejorar la lectura del codigo.
*/

bool minimumForce(int threshold){
  bool mf = (measure.raw() > threshold) ? true : false;
  return mf;
}

bool waitForUser(int period, const String& message){
  byte in;
  unsigned long time_now = millis();
  Serial.print(message);
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

float askTheUser(const String& message){
  float in;
  Serial.print(message);
  while(true){
    in = Serial.read();
  }
  return in;
}

void waitForMachine(int period){
  unsigned long time_now = millis();
  while(millis() < time_now + period);
}
