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
  
  // Logica de configuracion
  Serial.println("\n[MSJ]\tMonitor serie activado. Opciones de usuario.");
  conf = waitForUser(3000, "\n[INS]\t¿Reiniciar contador de lotes? [Y/N]: ");
  if(conf){
    EEPROM.put(memoryLocation[1], 0);
    EEPROM.commit();
    Serial.print("\n[CAL]\tLOTE: 0 (Reset)");
  }else{
    EEPROM.get(memoryLocation[1], lot);
    Serial.print("\n[CAL]\tLOTE: ");Serial.println(lot);
  }
  conf = waitForUser(3000, "\n[INS]\t¿Iniciar calibracion? [Y/N]: ");
  if(conf){
    patternWeight = askTheUser(5000, "\n[INS]\tIngrese el peso real del equipo: ");
    Serial.print("\n[CAL]\tPESO REAL: "); Serial.println(patternWeight);
    manualScale = measure.calibrate(patternWeight); // Se ingresa el peso real del equipo
    Serial.print("[CAL]\tESCALA: "); Serial.println(manualScale);
    EEPROM.put(memoryLocation[0], manualScale);
    EEPROM.commit();
  }else{
    Serial.println("\n[MSJ]\tCalibracion manual cancelada");
  }
  conf = waitForUser(3000, "[INS]\t¿Ingresar factor manualmente? [Y/N]: ");
  if(conf){
    manualScale = askTheUser(3000, "\n[INS]\tIngreses factor: ");
    Serial.print("\n[CAL]\tESCALA: "); Serial.println(manualScale);
    measure.manualSetup(manualScale);
    EEPROM.put(memoryLocation[0],manualScale);
    EEPROM.commit();
  }else{
    Serial.println("\n[MSJ]\tFactor manual cancelado.");
  }
  conf = waitForUser(3000, "[INS]\t¿Ingresar factor configurado? [Y/N]: ");
  if(conf){
    Serial.println("\n[MSJ]\tConfiguracion por programa.");
    EEPROM.put(memoryLocation[0], scale);
    EEPROM.commit();
    EEPROM.get(memoryLocation[0], scale);
    measure.manualSetup(scale);
    Serial.print("[CAL]\tESCALA: ");Serial.println(scale);
  }else{
    Serial.println("\n[MSJ]\tFactor por configuración cancelado.");
  }
  if(recorder.clock()){
    Serial.println("[OK]\tRTC");
    if(recorder.card()){
		sdModule = true;
      display.showImage(SDCARD);
      // Se agregan los titulos de archivos, que viene despues de los "Fecha" y "Hora" (titulos por defecto)
      recorder.setTitles(7, "Lote", "Unidad", "Fuerza", "Porcentaje", "Maximo", "Minimo", "Promedio");
	  Serial.println("[OK]\tSD");
    }else{
		sdModule = false;
      display.showImage(NOSD);
      Serial.println("[ERROR]\tSD");
    }
  }else{
    Serial.println("[ERROR]\tRTC");
  }
  sprintf(lot_buff, "%d", lot);
  recorder.saveRegistry(7, lot_buff, " ", " ", " ", " ", " ", " "); // Ejecucion estetica, no funcional
  Serial.println("[MSJ]\tConfiguracion terminada. Listo para medir.");
  EEPROM.end();
  // Fin de logica de configuracion 
  
  // Se configurar los pines usando métodos de Arduino
  pinMode(TOUCH, INPUT); // 0: No pulsado | 1: Pulsado
  attachInterrupt(digitalPinToInterrupt(CS), sd_card, CHANGE);
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
      sprintf(lot_buff, "%0.f", counter());
      recorder.saveRegistry(7, lot_buff, " ", " ", " ", " ", " ", " "); // Ejecucion estetica, no funcional
      Serial.print("[MJS]\tRegistro de lote cerrador.");Serial.print("\t|LOTE: ");Serial.println(lot_buff);
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
      in = Serial.parseFloat();
      break;
    }
  }
  Serial.print(in);
  return in;
}

void waitForMachine(int period){
  unsigned long time_now = millis();
  while(millis() < (time_now + period));
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

void sd_card(){
  unsigned long time_now = millis();
  sdModule = !sdModule;
  if(sdModule){
	display.showImage(SDCARD);
  }else{
	display.showImage(NOSD);
  }
  while(millis() < (time_now + 3000)
}

