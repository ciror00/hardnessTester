#include "hardnessTester.h"

Visualizer display;
LoadCell measure;
Recorder recorder;
DataHandler dataHandler;

void setup(){
  Serial.begin(115200);
  Serial.println("\n\"HARDNESS TESTER\"\nFirmware: "+ (String)FIRMWARE + \
                "\t| Environment: " + (String)ARDUINO + "\t| Compiler: "+ (String)__VERSION__);
  EEPROM.begin(memorySize); // Se reservar 32 Bytes (256 bits) | Tamaño maximo 4K (4096)
  display.begin();
  measure.begin(DT_CELL, SCK_CELL);
  display.showImage(COP);
  recorder.begin(CS);
  pinMode(TOUCH, INPUT); // 0: No pulsado | 1: Pulsado
  // Logica de configuracion
  if(!Serial){
    Serial.println("\n[MSJ]\tMonitor serie activado. Opciones de usuario.");
    conf = waitForUser(3000, "\n[INS]\t¿Reiniciar contador de lotes? [Y/N]: ");
    if(conf){
      EEPROM.put(memoryLocation[1], 0);
      EEPROM.commit();
    }else{
      EEPROM.get(memoryLocation[0], lot);
      Serial.print("[CAL]\tLOTE: ");Serial.println(lot);
    }
    conf = waitForUser(3000, "\n[INS]\t¿Iniciar calibracion? [Y/N]: ");
    if(conf){
      patternWeight = askTheUser(3000, "[INS]\tIngrese el peso real del equipo: ");
      Serial.print("[CAL]\tPESO REAL: "); Serial.println(patternWeight);
      manualScale = measure.calibrate(patternWeight); // Se ingresa el peso real del equipo
      Serial.print("[CAL]\tESCALA: "); Serial.println(manualScale);
      EEPROM.put(memoryLocation[0], manualScale);
      EEPROM.commit();
    }
    Serial.println("\n[MSJ]\tCalibracion manual cancelada");
    conf = waitForUser(3000, "[INS]\t¿Ingresar factor manualmente? [Y/N]: ");
    if(conf){
      manualScale = askTheUser(3000, "[INS]\tIngreses factor de escala: ");
      Serial.print("[CAL]\tESCALA: "); Serial.println(manualScale);
      measure.manualSetup(manualScale);
      EEPROM.put(memoryLocation[0],manualScale);
      EEPROM.commit();
    }else{
      Serial.println("\n[MSJ]\tConfiguracion manual cancelada. Configuracion por programa.");
      EEPROM.put(memoryLocation[0], scale);
      EEPROM.commit();
      EEPROM.get(memoryLocation[0], scale);
      Serial.print("[CAL]\tESCALA: ");Serial.print(scale);Serial.println(" (Default)");
    }
  }else{
    Serial.println("\n[MSJ]\tCargando configuracion guardada");
    EEPROM.get(memoryLocation[0], manualScale);
    measure.manualSetup(manualScale);
    //waitForMachine(2500);
    display.showMeasure((String)manualScale, "de escala");
    waitForMachine(2000);
  }
  if(recorder.clock()){
    Serial.println("[OK]\tRTC");
    if(recorder.card()){
      Serial.println("[OK]\tSD");
      // Se agregan los titulos de archivos, que viene despues de los "Fecha" y "Hora" (titulos por defecto)
      recorder.setTitles(7, "Lote", "Unidad", "Fuerza", "Porcentaje", "Maximo", "Minimo", "Promedio");
    }else{
      Serial.println("[ERROR]\tSD");
    }
  }else{
    Serial.println("[ERROR]\tRTC");
  }
  waitForMachine(1000);
  //EEPROM.get(memoryLocation[0], lot);
  recorder.saveRegistry(7, lot, " ", " ", " ", " ", " ", " "); // Ejecucion estetica, no funcional
  Serial.println("[MSJ]\tConfiguracion terminada.");
  EEPROM.end();
}

void loop(){
  button = digitalRead(TOUCH);
  if(button == 1){
    Serial.println("[MJS]\tListo para medir");
    if(minimumForce(sensibility)){
      flag = true;
      fruit++;
      while(minimumForce(sensibility)){
        strength = measure.strengthAverage(stabilizer);
        Serial.print("[CAL]\tFUERZA: ");Serial.print(strength);Serial.print("|\tSEÑAL: ");Serial.println(measure.raw());
        display.showMeasure((String)strength, "kgf");
        sum += strength;
        count++;
      };
      display.showMessage("Procesando...");
      averange = sum / count;
      dataHandler.preLoad(averange);
      sprintf(count_buff, "%d", fruit);
      sprintf(strength_buff, "%.0f", averange);
      sprintf(percentages_buff, "%.0f", dataHandler.percentages());
      //|Columnas| "Lote", "Unidad", "Fuerza", "Porcentaje", "Maximo", "Minimo", "Promedio"
      recorder.saveRegistry(7, " ", count_buff, strength_buff, percentages_buff, " ", " ", " ");
      count = 0;
      Serial.print("[CAL]\tMEDICION: ");Serial.println(strength_buff);
    }else{
      display.showMessage("Listo para medir");
    }
  }else{
    display.showImage(PUSH);
    if(flag){
      flag = false;
      fruit = 0;
      sprintf(max_buff, "%.0f", dataHandler.maximum());
      sprintf(min_buff, "%.0f", dataHandler.minimum());
      sprintf(average_buff, "%.0f", dataHandler.average());
      //|Columnas| "Lote", "Unidad", "Fuerza", "Porcentaje", "Maximo", "Minimo", "Promedio"
      recorder.saveRegistry(7, " ", " ", " ", " ", max_buff, min_buff, average_buff);
      Serial.println("[MJS]\tResumen de datos calculados");
      Serial.print("\tMAXIMA: ");Serial.println(max_buff);
      Serial.print("\tMINIMA: ");Serial.println(min_buff);
      Serial.print("\tPROMEDIO: ");Serial.println(average_buff);
      dataHandler.reset();
      sprintf(lot_buff, "%.0f", counter());
      recorder.saveRegistry(7, lot, " ", " ", " ", " ", " ", " "); // Ejecucion estetica, no funcional
      Serial.println("[MJS]\tNuevo Lote iniciado.");Serial.print("|\tLOTE: ");Serial.println(lot);
    }
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

float askTheUser(int period, const String& message){
  float in;
  unsigned long time_now = millis();
  Serial.print(message);
  while(millis() < time_now + period){
    if (Serial.available() > 0) {
      in = Serial.read();
      break;
    }
  return in;
  }
}

void waitForMachine(int period){
  unsigned long time_now = millis();
  while(millis() < time_now + period);
}

int counter(){
  int number;
  EEPROM.get(memoryLocation[1], number);
  number++;
  EEPROM.put(memoryLocation[1],number);
  EEPROM.commit();
  EEPROM.end();
  return number;
}
