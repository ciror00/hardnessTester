#include "hardnessTester.h"

Visualizer display;
LoadCell measure;
Recorder recorder;
DataHandler dataHandler;

void setup(){
  Serial.begin(115200);
  Serial.println("\n\"HARDNESS TESTER\"\nFirmware: "+ (String)FIRMWARE +"\t| Environment: " + (String)ARDUINO + "\t| Compiler: "+ (String)__VERSION__);
  EEPROM.begin(32); // Se reservar 32 Bytes (256 bits) | Tamaño maximo 4K (4096)
  display.begin();
  measure.begin(DT_CELL, SCK_CELL);
  display.showImage(COP);
  conf = waitForUser(3000, "\n[INS]\t¿Iniciar calibracion? [Y/N]: ");
  if(conf){
    patternWeight = askTheUser("[INS]\tIngrese el peso real del equipo: ");
    Serial.print("[CAL]\tPESO REAL: "); Serial.println(patternWeight);
    manualScale = measure.calibrate(patternWeight); // Se ingresa el peso real del equipo
    Serial.print("[CAL]\tESCALA: "); Serial.println(manualScale);
    EEPROM.put(0, manualScale);
    EEPROM.commit();
  }
  Serial.println("\n[MSJ]\tCalibracion manual cancelada");
  conf = waitForUser(3000, "[INS]\t¿Ingresar factor manualmente? [Y/N]: ");
  if(conf){
    manualScale = askTheUser("[INS]\tIngreses factor de escala: ");
    Serial.print("[CAL]\tESCALA: "); Serial.println(manualScale);
    measure.manualSetup(manualScale);
    EEPROM.put(0,manualScale);
    EEPROM.commit();
  }else{
    EEPROM.put(0,scale);
    EEPROM.commit();
  }
  Serial.println("\n[MSJ]\tConfiguracion manual cancelada");
  EEPROM.get(0, scale);
  Serial.print("[CAL]\tESCALA: ");Serial.println(scale);
  pinMode(TOUCH, INPUT); // 0: No pulsado | 1: Pulsado
  if(recorder.begin(CS)){
    Serial.println("[OK]\tSD & Reloj");
    // Se agregan los titulos de archivos, que viene despues de los "Fecha" y "Hora" (titulos por defecto)
    recorder.setTitles(7, "Lote", "Unidad", "Fuerza", "Porcentaje", "Maximo", "Minimo", "Promedio");
  }else{
    Serial.println("[ERROR]\tSD & Reloj");
  }
  waitForMachine(1000);
  sprintf(lot, "%ld", random(10000, 100000));
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
        Serial.print("[CAL]\tFUERZA: ");Serial.println(strength);
        display.showMeasure((String)strength, "kgf");
        sum += strength;
        count++;
      };
      display.showMessage("Procesando...");
      averange = sum / count;
      dataHandler.preLoad(averange);
      sprintf(count_buff, "%d", fruit);
      sprintf(strength_buff, "%.3f", averange);
      sprintf(percentages_buff, "%.3f", dataHandler.percentages());
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
      sprintf(max_buff, "%.3f", dataHandler.maximum());
      sprintf(min_buff, "%.3f", dataHandler.minimum());
      sprintf(average_buff, "%.3f", dataHandler.average());
      //|Columnas| "Lote", "Unidad", "Fuerza", "Porcentaje", "Maximo", "Minimo", "Promedio"
      recorder.saveRegistry(7, " ", " ", " ", " ", max_buff, min_buff, average_buff);
      Serial.println("[MJS]\tResumen de datos calculados");
      Serial.print("[MJS]\tMAXIMA: ");Serial.println(max_buff);
      Serial.print("[MJS]\tMINIMA: ");Serial.println(min_buff);
      Serial.print("[MJS]\tPROMEDIO: ");Serial.println(average_buff);
      dataHandler.reset();
      sprintf(lot, "%ld", random(10000, 100000));
      recorder.saveRegistry(7, lot, " ", " ", " ", " ", " "); // Ejecucion estetica, no funcional
      Serial.println("[MJS]\tNuevo Lote iniciado");
      Serial.print("[CAL]\tLOTE: ");Serial.println(lot);
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
