#include "ground.h"

LiteVisualizer display;
LoadCell measure;
Recorder recorder;
DataHandler forceAnalyzer;
DataHandler distanceAnalyzer;

void setup(){
  Serial.begin(115200);
  serial_gps.begin(9600);
  Serial.println("\n\"HARDNESS TESTER (ground)\"\nFirmware: "+ (String)FIRMWARE + \
                "\t| Environment: " + (String)ARDUINO + "\t| Compiler: "+ (String)__VERSION__);
  display.begin();
  display.switcher(true);
  display.showMessage("COPAIN SRL");
  measure.begin(DT_CELL, SCK_CELL);
  recorder.begin(CS);

  // Configuacion de modulo
  if(recorder.clock()){
    Serial.println("[OK]\tRTC operativo");
    if(recorder.card()){
		sdModule = true;
      //display.showImage(CARD, "SD operativa");
	  Serial.println("[OK]\tSD");
    }else{
			sdModule = false;
      //display.showImage(NOCARD, "Error en SD");
      Serial.println("[ERROR]\tSD");
    }
  }else{
    Serial.println("[ERROR]\tRTC");
  }

  // Menu de configuración
  Serial.println("[MSJ]\tIngrese un peso para iniciar la calibración automática.");
  scaleCalculation = askTheUser("[INS]\tPeso: ", waitConfiguration);
  if(scaleCalculation > 0){
		manualScale = measure.calibrate(scaleCalculation);
		Serial.print("\n[CAL]\tFACTOR: "); Serial.println(manualScale);
    EEPROM.write(memoryLocation[0], manualScale);
		EEPROM.write(memoryLocation[1], 1);
		Serial.println("[MJS]\tMedicion: 1 (contador reiniciado)");
		Serial.println("\n[MJS]\tReinicia el equipo manualmente...");
    delay(99999);
  }else if(scaleCalculation == 0){
		Serial.println("\n[MSJ]\tCalibración automática cancelada.");
    Serial.println("[MSJ]\tCalibración por defecto.");
    manualScale = scale;
    EEPROM.write(memoryLocation[1], 1);
		Serial.println("[MJS]\tMedicion: 1 (contador reiniciado)");
    EEPROM.write(memoryLocation[0], manualScale);
    Serial.print("[CAL]\tFACTOR: "); Serial.println(manualScale);
    measure.manualSetup(manualScale);
  }else if(scaleCalculation == -1){
		Serial.println("\n[MSJ]\tCargando configuracion guardada.");
    EEPROM.get(memoryLocation[0], manualScale);
    Serial.print("[MSJ]\tFACTOR: "); Serial.println(manualScale);
    measure.manualSetup(manualScale);
    EEPROM.get(memoryLocation[1], lot);
    Serial.print("[CAL]\tMedicion: ");Serial.println(lot);
	}

  Serial.println("[MSJ]\tConfiguranndo GPS");
  if(serial_gps.available()){
    Serial.println("[MSJ]\tPuerto serie conectado a Modulo GPS");
    // Se realiza un "ping" a modulo GSP
    int c = serial_gps.read();
    if (gps.encode(c))geo=true;
    // Se obtienen estadisticas del sensor
    gps.stats(&chars, &sentences, &failed);
    Serial.print(">Estaditicas: char: "); Serial.print(chars); Serial.print(" sentences: ");
    Serial.print(sentences); Serial.print(" failed checksum: "); Serial.println(failed);
    // Se obtiene posocion, fecha y hora
    Serial.println("[MSJ]\tObteniendo geolocalización");
    gps.f_get_position(&flat, &flon, &age); // Obtengo posicion
    Serial.print(">Lat: "); Serial.print(flat);Serial.print("\t|Lon: "); Serial.println(flon);
    sprintf(lat_buff, "%d", flat);
    sprintf(lon_buff, "%d", flon);
    Serial.println("[MSJ]\tSincronizando RTC");
    gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age); // Obtengo fecha y hora
    if(year!=0)recorder.setDate(year,month,day,hour,minute);
  }else{
    Serial.println("[ERROR]\tFalla de comunicación serie");
    Serial.println("[MSJ]\tFecha y hora por defecto.");
  }
  recorder.showTime();

	// Se agregan los titulos de archivos, que viene despues de los "Fecha" y "Hora" (titulos por defecto)
	//|Columnas| {Medicion", "Latitud", "Longitud", "Distancia", "Dist. Max", "F. Maxima", "F. Minima", "F. Promedio"}
  headers= recorder.setTitles(sizeof(titles), titles[0], titles[1], titles[2], titles[3], titles[4], titles[5], titles[6], titles[7]);
  sprintf(lot_buff, "%d", lot);
 	recorder.saveRegistry(sizeof(titles), lot_buff, " ", " ", " ", " ", " ", " ", " "); // Ejecucion estetica, no funcional

  // Se configurar los pines usando métodos de Arduino
  Serial.println("[MSJ]\tMidiendo jabalina.");
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  range = rule.ping_cm();
  Serial.print("[CAL]\tLargo de lanza medida: ");Serial.println(range);
  Serial.println("[MSJ]\tConfiguracion terminada. Listo para medir.");
}

void loop(){
  display.switcher(false);
  if(minimumForce(sensibility)){
    if(close){
      close = false;
      sprintf(lot_buff, "%d", lot);
      recorder.saveRegistry(sizeof(titles), lot_buff, " ", " ", " ", " ", " ", " ", " ", " "); // Ejecucion estetica, no funcional
      Serial.print("[CAL]\tMEDICION: ");Serial.println(lot);
    }
    flag = true;
    strength = measure.strengthAverage(stabilizer);
    Serial.print("[CAL]\tFUERZA: ");Serial.print(strength);
    Serial.print("|\tSEÑAL: ");Serial.println(measure.raw());
    // Medicion de distancia
    reducible = rule.ping_cm();
    Serial.print("[CAL]\tALTURA: ");Serial.print(reducible);
    depth = range - reducible;
    Serial.print("|\tPROFUNDIDAD: ");Serial.println(depth);
    sprintf(strength_buff, "%.2f", strength);sprintf(depth_buff, "%.2f", reducible);
    display.showMeasure(1, strength_buff);
    display.showMeasure(2, depth_buff);
    forceAnalyzer.preLoad(strength);
    distanceAnalyzer.preLoad(reducible);
    if(!sdModule){
      Serial.println("[ERROR]\tSD mal configurada");
    }else{
      Serial.println("[MJS]\tGuardando en SD");
      //|Columnas| {Medición", "Distancia", "Latitud", "Longitud", "Dist. Max", "F. Maxima", "F. Minima", "F. Promedio"}
      recorder.saveRegistry(sizeof(titles), lot, strength_buff, range_buff, lat_buff, lon_buff, " ", " ", " ", " ");
    }
    display.showMeasure(1, strength_buff);
    display.showMeasure(2, range_buff);
    delay(2000);
    strength = 0;
  }else{
    display.showMessage("Listo para medir");
  }
  if(flag){
    display.showMessage("Procesando...");
    flag = false;
    sprintf(max_buff, "%.2f", forceAnalyzer.maximum());
    sprintf(min_buff, "%.2f", forceAnalyzer.minimum());
    sprintf(average_buff, "%.2f", forceAnalyzer.average());
    sprintf(range_buff, "%.2f", distanceAnalyzer.maximum());
    if(!recorder.card()){
      //display.showImage(NOCARD, "Error en SD");
      Serial.println("[ERROR]\tSD no reconocida");
    }else{
      Serial.println("[MJS]\tGuardando en SD");
      //|Columnas| {Medición", "Distancia", "Latitud", "Longitud", "Dist. Max", "F. Maxima", "F. Minima", "F. Promedio"}
      recorder.saveRegistry(6, lot_buff, " ", " ", " ", range_buff, max_buff, min_buff, average_buff);
    }
    Serial.println("[MJS]\tResumen de datos calculados");
    Serial.print("\t> PROFUNDIDAD: ");Serial.println(range_buff);
    Serial.print("\t> F. MAXIMA: ");Serial.println(max_buff);
    Serial.print("\t> F. MINIMA: ");Serial.println(min_buff);
    Serial.print("\t> F. PROMEDIO: ");Serial.println(average_buff);
    forceAnalyzer.reset();
    distanceAnalyzer.reset();
    delay(1000);
    if(!recorder.card()){
      //display.showImage(NOCARD, "Error en SD");
      Serial.println("[ERROR]\tSD no reconocida");
    }else{
      Serial.println("[MJS]\tGuardando en SD");
    }
    Serial.print("[MJS]\tRegistro de medicion cerrador.");
    lot = counter();
    close = true;
  }
}

/*
  Se encapsulan funciones para mejorar la lectura del codigo.
*/

bool minimumForce(int threshold){
  display.switcher(true);
  int t = 0;
  float s = threshold;
  while(s >= threshold && t < 5){
    t++;
    s = measure.strength();
    Serial.print("> SENSOR: ");Serial.println(s);
  }
	bool mf = (t > 5) ? true : false;
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

unsigned int counter(){
  unsigned int number;
  EEPROM.get(memoryLocation[1], number);
	number++;
  EEPROM.write(memoryLocation[1],number);
  return number;
}
