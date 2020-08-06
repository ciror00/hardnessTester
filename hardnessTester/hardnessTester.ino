#include "hardnessTester.h"

Visualizer display;
LoadCell measure;
Recorder recorder;
DataHandler dataHandler;

void setup(){
	waitForMachine(6000);
  Serial.begin(115200);
  Serial.println("\n\"HARDNESS TESTER\"\nFirmware: "+ (String)FIRMWARE + \
                "\t| Environment: " + (String)ARDUINO + "\t| Compiler: "+ (String)__VERSION__);
  EEPROM.begin(memorySize); // Se reservar 32 Bytes (256 bits) | Tamaño maximo 4K (4096)
  display.begin();
  display.showImage(COP);
  measure.begin(DT_CELL, SCK_CELL);
  recorder.begin(CS);

  // Menu de configuración
  Serial.println("\n[MSJ]\tIngrese un peso para iniciar la calibración automática.");
  scaleCalculation = askTheUser("[INS]\tPeso: ", waitConfiguration);
  if(scaleCalculation > 0){
    display.showImage(TOOL);
		manualScale = -(measure.calibrate(scaleCalculation));
		Serial.print("\n[CAL]\tFACTOR: "); Serial.println(manualScale);
    EEPROM.put(memoryLocation[0], manualScale);
    EEPROM.commit();
    EEPROM.end();
		Serial.println("\n[MJS]\tReiniciando equipo...");
		waitForMachine(waitConfiguration);
		ESP.restart();
  }else if(scaleCalculation == 0){
		Serial.println("\n[MSJ]\tCalibración automática cancelada.");
    Serial.println("\n[MSJ]\tCalibración por defecto.");
    manualScale = scale;
    Serial.print(manualScale);
		EEPROM.put(memoryLocation[1], 1);
		Serial.println("\n[MJS]\tLOTE: 1");
    Serial.print("[CAL]\tFACTOR: "); Serial.println(manualScale);
    EEPROM.put(memoryLocation[0], manualScale);
    EEPROM.commit();
  }else if(scaleCalculation == -1){
		Serial.println("\n[MSJ]\tCargando configuracion guardada.");
    EEPROM.get(memoryLocation[0], manualScale);
    Serial.print("[MSJ]\tFACTOR: "); Serial.println(manualScale);
    measure.manualSetup(manualScale);
    EEPROM.get(memoryLocation[1], lot);
    Serial.print("[CAL]\tLOTE: ");Serial.println(lot);
	}

	// Configuacion de modulo
  if(recorder.clock()){
    Serial.println("[OK]\tRTC");
    recorder.showTime();
    if(recorder.card()){
		sdModule = true;
      display.showImage(CARD, "SD operativa");
      // Se agregan los titulos de archivos, que viene despues de los "Fecha" y "Hora" (titulos por defecto)
      recorder.setTitles(6, "Lote", "Unidad", "Fuerza", "Maximo", "Minimo", "Promedio");
	  Serial.println("[OK]\tSD");
    }else{
			sdModule = false;
      display.showImage(NOCARD, "Error en SD");
      Serial.println("[ERROR]\tSD");
    }
  }else{
    Serial.println("[ERROR]\tRTC");
  }
  waitForMachine(9000);
  sprintf(lot_buff, "%d", lot);
	//|Columnas| "Lote", "Unidad", "Fuerza", "Porcentaje", "Maximo", "Minimo", "Promedio"
  recorder.saveRegistry(6, lot_buff, " ", " ", " ", " ", " "); // Ejecucion estetica, no funcional
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
        disposable = measure.strengthAverage(stabilizer);
        Serial.print("[CAL]\tFUERZA: ");Serial.print(disposable);
        Serial.print("|\tSEÑAL: ");Serial.println(measure.raw());
	      if(disposable < 0)disposable = 0;
        sprintf(disposable_buff, "%.0f g", disposable);
        display.showMeasure(disposable_buff);
        //sum += strength;
        //count++;
				if(disposable >= strength)strength = disposable;
      };
      display.showMessage("Procesando...");
      //averange = sum / count;
      dataHandler.preLoad(strength);
      sprintf(count_buff, "%d", fruit);
			sprintf(force_buff, "%.0f g", strength);
      sprintf(strength_buff, "%.0f", strength);
			//sprintf(percentages_buff, "%.2f", dataHandler.percentages(elasticLimit, strength));
      if(!sdModule){
        display.showMeasure(force_buff, " ", "No guardado. Error en SD");
        Serial.println("[ERROR]\tSD mal configurada");
				waitForUser(" ", showMeasure);
      }else{
        Serial.println("[MJS]\tGuardando en SD");
				//|Columnas| "Lote", "Unidad", "Fuerza", "Maximo", "Minimo", "Promedio"
        recorder.saveRegistry(6, " ", count_buff, strength_buff, " ", " ", " ");
	      display.showMeasure(force_buff, " ", "Guardado");
      }
			waitForUser(" ", showMeasure);
      //count = 0;
      Serial.print("[CAL]\tMEDICION: ");Serial.println(strength);
			strength = 0;
    }else{
      disposable = measure.strength();
      Serial.print("[CAL]\tFUERZA: ");Serial.print(disposable);
      Serial.print("|\tSEÑAL: ");Serial.print(measure.raw());Serial.println("|\tFuerza insuficiente");
	    if(disposable < 0)disposable = 0;
      sprintf(disposable_buff, "0 g"); // Se fija un cero para que se muestre por pantalla
      display.showMeasure(disposable_buff);
    }
  }else{
    if(flag){
			display.showMessage("Cerrando lote...");
      flag = false;
      fruit = 0;
      sprintf(max_buff, "%.0f", dataHandler.maximum());
      sprintf(min_buff, "%.0f", dataHandler.minimum());
      sprintf(average_buff, "%.2f", dataHandler.average());
      //sprintf(percentages_buff, "%.2f", dataHandler.percentages(dataHandler.maximum(), dataHandler.minimum()));
      if(!recorder.card()){
        display.showImage(NOCARD, "Error en SD");
        Serial.println("[ERROR]\tSD no reconocida");
      }else{
        Serial.println("[MJS]\tGuardando en SD");
				//|Columnas| "Lote", "Unidad", "Fuerza", "Maximo", "Minimo", "Promedio"
        recorder.saveRegistry(6, " ", " ", " ", max_buff, min_buff, average_buff);
      }
      Serial.println("[MJS]\tResumen de datos calculados");
      Serial.print("\tMAXIMA: ");Serial.println(max_buff);
      Serial.print("\tMINIMA: ");Serial.println(min_buff);
      Serial.print("\tPROMEDIO: ");Serial.println(average_buff);
      //Serial.print("\tPORCENTAJE: ");Serial.println(percentages_buff);
      dataHandler.reset();
      sprintf(lot_buff, "%0.f", counter());
      if(!recorder.card()){
        display.showImage(NOCARD, "Error en SD");
        Serial.println("[ERROR]\tSD no reconocida");
      }else{
        Serial.println("[MJS]\tGuardando en SD");
				//|Columnas| "Lote", "Unidad", "Fuerza", "Porcentaje", "Maximo", "Minimo", "Promedio"
        recorder.saveRegistry(6, lot_buff, " ", " ", " ", " ", " "); // Ejecucion estetica, no funcional
      }
      Serial.print("[MJS]\tRegistro de lote cerrador.");Serial.print("\t|LOTE: ");Serial.println(lot_buff);
    }
		display.showImage(PUSH);
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
  //bool mf = (measure.raw() > threshold) ? true : false;
	bool mf = (measure.strength() > threshold) ? true : false;
  return mf;
}

bool waitForUser(const String& message, int period){
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

float askTheUser(const String& message, int period){
  float in = -1;
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
	int wait = 0;
  while(wait < period){
		wait++;
	};
}

float counter(){
  float number;
  EEPROM.begin(memorySize);
  EEPROM.get(memoryLocation[1], number);
	number++;
  EEPROM.put(memoryLocation[1],number);
  EEPROM.commit();
  EEPROM.end();
  return number;
}
