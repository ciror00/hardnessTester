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
  display.showImage(COP);
  measure.begin(DT_CELL, SCK_CELL);
  recorder.begin(CS);

  Serial.println("[MSJ]\tAntes de iniciar el modo configuracion: \n\t - Recuerde que debe estar pre cargado el peso real del equipo.\n\t - El equipo mismo deberá pesarse apoyandolo sobre la galga.");
  conf = waitForUser(between, "\n[INS]\tIngrese cualquier tecla para formatear el equipo: ");
  if(conf){
    EEPROM.put(memoryLocation[1], 0);
    EEPROM.commit();
    Serial.println("\n[MSJ]\tLote reiniciado.");
    Serial.print("[MSJ]\tIniciando calibracion. Peso: "); Serial.println(patternWeight);
    display.showMessage("Pesar el equipo");
    manualScale = measure.calibrate(patternWeight); // Se ingresa el peso real del equipo
    if(manualScale == 0){
      Serial.println("\n[MSJ]\tFactor incorrecto. Utilizando factor pre cargado.");
      manualScale = scale;
    }
    Serial.print("[CAL]\tFACTOR: "); Serial.println(manualScale);
    Serial.print("[MJS]\tReiniciando el equipo...");
    display.showMessage("Reiniciando...");
    EEPROM.put(memoryLocation[0], manualScale);
    EEPROM.commit();
    EEPROM.end();
    while(wait < 9000){wait++;};
    ESP.restart();
  }else{
    Serial.println("\n[MSJ]\tCargando configuracion guardada.");
    EEPROM.get(memoryLocation[0], manualScale);
    Serial.print("\n[CAL]\tFACTOR: "); Serial.print(manualScale);
    measure.manualSetup(manualScale);
    EEPROM.get(memoryLocation[1], lot);
    Serial.print("\n[CAL]\tLOTE: ");Serial.println(lot);
  }
  /*
  if(conf){
    patternWeight = askTheUser(between, "\n[INS]\tIngrese el peso real del equipo: ");
    Serial.print("\n[CAL]\tPESO REAL: "); Serial.println(patternWeight);
    manualScale = measure.calibrate(patternWeight); // Se ingresa el peso real del equipo
    Serial.print("[CAL]\tFactor calculado: "); Serial.println(manualScale);
    EEPROM.put(memoryLocation[0], manualScale);
    EEPROM.commit();
  }else{
    Serial.println("\n[MSJ]\tCalibracion por pasos no iniciada.");
    conf = waitForUser(between, "[INS]\t¿Realizar calibracion por defecto? [Y/N]: ");
    if(conf){
      EEPROM.put(memoryLocation[0], scale);
      manualScale = scale;
      EEPROM.put(memoryLocation[1], 0);
      EEPROM.commit();
      Serial.print("\n[MSJ]\tLote reiniciado. Escala configurada por defecto.");
    }else{
      Serial.print("\n[MSJ]\tUtilizando factor guardado.");
      EEPROM.get(memoryLocation[0], manualScale);
    }
  }
  */
  if(recorder.clock()){
    Serial.println("[OK]\tRTC");
    if(recorder.card()){
		sdModule = true;
      display.showImage(CARD);
      // Se agregan los titulos de archivos, que viene despues de los "Fecha" y "Hora" (titulos por defecto)
      recorder.setTitles(7, "Lote", "Unidad", "Fuerza", "Porcentaje", "Maximo", "Minimo", "Promedio");
	  Serial.println("[OK]\tSD");
    }else{
		sdModule = false;
      display.showImage(NOCARD);
      Serial.println("[ERROR]\tSD");
    }
  }else{
    Serial.println("[ERROR]\tRTC");
  }
  while(wait < 3000){wait++;};
  sprintf(lot_buff, "%d", lot);
  recorder.saveRegistry(7, lot_buff, " ", " ", " ", " ", " ", " "); // Ejecucion estetica, no funcional
  Serial.println("[MSJ]\tConfiguracion terminada. Listo para medir.");
  EEPROM.end();

  // Se configurar los pines usando métodos de Arduino
  pinMode(TOUCH, INPUT); // 0: No pulsado | 1: Pulsado
}

void loop(){
  if(digitalRead(TOUCH))switcher();
  if(button == true){
    if(minimumForce(sensibility)){
      flag = true;
      fruit++;
      while(minimumForce(sensibility)){
        strength = measure.strengthAverage(stabilizer);
        Serial.print("[CAL]\tFUERZA: ");Serial.print(strength);
        Serial.print("|\tSEÑAL: ");Serial.println(measure.raw());
        display.showMeasure((String)strength, "kgf");
        sum += strength;
        count++;
      };
      display.showMessage("Procesando...");
      averange = sum / count;
      dataHandler.preLoad(averange);
      sprintf(count_buff, "%d", fruit);
      sprintf(strength_buff, "%.0f", averange);
      sprintf(percentages_buff, "%.0f", dataHandler.percentages(1, 0));
      //|Columnas| "Lote", "Unidad", "Fuerza", "Porcentaje", "Maximo", "Minimo", "Promedio"
      if(!recorder.card()){
        display.showImage(NOCARD);
        Serial.println("[ERROR]\tSD");
      }else{
        Serial.println("[MJS]\tGuardando en SD");
        recorder.saveRegistry(7, " ", count_buff, strength_buff, percentages_buff, " ", " ", " ");
      }
      count = 0;
      Serial.print("[CAL]\tMEDICION: ");Serial.println(strength_buff);
    }else{
      Serial.print("[CAL]\tFUERZA: ");Serial.print(measure.strength());
      Serial.print("|\tSEÑAL: ");Serial.print(measure.raw());Serial.println("|\t(Sin guardar)");
      display.showMeasure((String)strength, "kgf");
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
      if(!recorder.card()){
        display.showImage(NOCARD);
        Serial.println("[ERROR]\tSD");
      }else{
        Serial.println("[MJS]\tGuardando en SD");
        recorder.saveRegistry(7, " ", " ", " ", " ", max_buff, min_buff, average_buff);
      }
      Serial.println("[MJS]\tResumen de datos calculados");
      Serial.print("\tMAXIMA: ");Serial.println(max_buff);
      Serial.print("\tMINIMA: ");Serial.println(min_buff);
      Serial.print("\tPROMEDIO: ");Serial.println(average_buff);
      dataHandler.reset();
      sprintf(lot_buff, "%0.f", counter());
      if(!recorder.card()){
        display.showImage(NOCARD);
        Serial.println("[ERROR]\tSD");
      }else{
        Serial.println("[MJS]\tGuardando en SD");
        recorder.saveRegistry(7, lot_buff, " ", " ", " ", " ", " ", " "); // Ejecucion estetica, no funcional
      }
      Serial.print("[MJS]\tRegistro de lote cerrador.");Serial.print("\t|LOTE: ");Serial.println(lot_buff);
    }
  }
}

/*
  Se encapsulan funciones para mejorar la lectura del codigo.
*/
void switcher(){
  button = !button;
  Serial.print("[CAL]\tEstado del botón cambiado: ");Serial.println(button);
  while(digitalRead(TOUCH) == HIGH);
}

bool minimumForce(int threshold){
  bool mf = (measure.raw() > threshold) ? true : false;
  return mf;
}

bool waitForUser(int period, const String& message){
  byte in = 0;
  unsigned long time_now = millis();
  Serial.print(message);
  while(millis() < time_now + period){
    if (Serial.available() > 0) {
      in = Serial.read();
      break;
    }
  }
  if(in != 0){
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
      in = Serial.parseFloat();
      break;
    }
  }
  Serial.print(in);
  return in;
}

void waitForMachine(int period){
  unsigned long time_now = millis();
  while(millis() < time_now + period);
}

float counter(){
  float number;
  EEPROM.begin(memorySize);
  EEPROM.get(memoryLocation[1], number);
  EEPROM.put(memoryLocation[1],number+1);
  EEPROM.commit();
  EEPROM.end();
  return number;
}
