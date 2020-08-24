#include "ground.h"

LiteVisualizer display;
//Visualizer display;
LoadCell measure;
Recorder recorder;
DataHandler forceAnalyzer;
DataHandler distanceAnalyzer;

void setup(){
	//waitForMachine(6000);
  Serial.begin(115200);
  serial_gps.begin(9600);
  Serial.println("\n\"HARDNESS TESTER (ground)\"\nFirmware: "+ (String)FIRMWARE + \
                "\t| Environment: " + (String)ARDUINO + "\t| Compiler: "+ (String)__VERSION__);
  //EEPROM.begin(memorySize); // Se reserva espacio en memoria | Tamaño maximo 4K (4096)
  display.begin();
  display.switcher(1);
  //display.showImage(COP);
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
  //waitForMachine(9000);

  // Menu de configuración
  Serial.println("[MSJ]\tIngrese un peso para iniciar la calibración automática.");
  scaleCalculation = askTheUser("[INS]\tPeso: ", waitConfiguration);
  if(scaleCalculation > 0){
    //display.showImage(TOOL);
		manualScale = measure.calibrate(scaleCalculation);
		Serial.print("\n[CAL]\tFACTOR: "); Serial.println(manualScale);
    //EEPROM.put(memoryLocation[0], manualScale);
		//EEPROM.put(memoryLocation[1], 1);
    EEPROM.update(memoryLocation[0], manualScale);
		EEPROM.update(memoryLocation[1], 1);
		Serial.println("[MJS]\tMedicion: 1");
    //EEPROM.commit();
    //EEPROM.end();
		Serial.println("\n[MJS]\tReinicia el equipo manualmente...");
    delay(99999);
		//waitForMachine(waitConfiguration);
		//ESP.restart();
  }else if(scaleCalculation == 0){
		Serial.println("\n[MSJ]\tCalibración automática cancelada.");
    Serial.println("[MSJ]\tCalibración por defecto.");
    manualScale = scale;
		//EEPROM.put(memoryLocation[1], 1);
    EEPROM.update(memoryLocation[1], 1);
		Serial.println("[MJS]\tMedicion: 1");
    Serial.print("[CAL]\tFACTOR: "); Serial.println(manualScale);
    //EEPROM.put(memoryLocation[0], manualScale);
    //EEPROM.commit();
    EEPROM.update(memoryLocation[0], manualScale);
	  //Serial.println("[MSJ]\tConfigurando hora por defecto.");
	  //if(dateTimeSetting)recorder.setDate(dates[2],dates[1],dates[0],times[0],times[1]);
    //recorder.showTime();
  }else if(scaleCalculation == -1){
		Serial.println("\n[MSJ]\tCargando configuracion guardada.");
    EEPROM.get(memoryLocation[0], manualScale);
    Serial.print("[MSJ]\tFACTOR: "); Serial.println(manualScale);
    measure.manualSetup(manualScale);
    EEPROM.get(memoryLocation[1], lot);
    Serial.print("[CAL]\tMedicion: ");Serial.println(lot);
	  //recorder.showTime();
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
  //EEPROM.end();

  // Se configurar los pines usando métodos de Arduino
  Serial.println("\n[MSJ]\tMidiendo jabalina.");
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  range = rule.ping_cm();
  Serial.print("\n[CAL]\tLargo de lanza medida: ");Serial.println(range);
  Serial.println("\n[MSJ]\tConfiguracion terminada. Listo para medir.");
}

void loop(){
  display.switcher(0);
  //if(digitalRead(TOUCH))switcher();
  //if(button == true){
  if(minimumForce(sensibility)){
    if(close){
      close = false;
      sprintf(lot_buff, "%d", lot);
      recorder.saveRegistry(sizeof(titles), lot_buff, " ", " ", " ", " ", " ", " ", " ", " "); // Ejecucion estetica, no funcional
      Serial.print("[CAL]\tMEDICION: ");Serial.println(lot);
    }
    display.switcher(1);
    flag = true;
    //while(minimumForce(sensibility)){
      //disposable = measure.strengthAverage(stabilizer);
      // Medicion de fuerza
      strength = measure.strengthAverage(stabilizer);
      Serial.print("[CAL]\tFUERZA: ");Serial.print(disposable);
      Serial.print("|\tSEÑAL: ");Serial.println(measure.raw());
      // Medicion de distancia
      reducible = rule.ping_cm();
      Serial.print("[CAL]\tALTURA: ");Serial.print(reducible);
      depth = range - reducible;
      Serial.print("|\tPROFUNDIDAD: ");Serial.print(depth);
      //if(disposable < sensibility)disposable = sensibility;
      //sprintf(disposable_buff, "%.0f", disposable);
      //display.showMeasure(disposable_buff);
      //if(disposable >= strength)strength = disposable;
    //};
    //display.showMessage("Procesando...");
    //if(strength == 0)strength = sensibility; // Parche para evitar ceros en CSV
    sprintf(strength_buff, "%.2f", strength);sprintf(depth_buff, "%.2f", reducible);
    display.showMeasure(1, strength_buff);
    display.showMeasure(2, depth_buff);
    forceAnalyzer.preLoad(strength);
    distanceAnalyzer.preLoad(reducible);
    //sprintf(count_buff, "%d", ground);
    //sprintf(force_buff, "%.0f g", strength);
    //sprintf(strength_buff, "%.0f", strength);
    if(!sdModule){
      //display.showMeasure(force_buff, " ", "No guardado. Error en SD");
      Serial.println("[ERROR]\tSD mal configurada");
      waitForUser(" ", showMeasure);
    }else{
      Serial.println("[MJS]\tGuardando en SD");
      //|Columnas| {Medición", "Distancia", "Latitud", "Longitud", "Dist. Max", "F. Maxima", "F. Minima", "F. Promedio"}
      recorder.saveRegistry(sizeof(titles), lot, strength_buff, range_buff, lat_buff, lon_buff, " ", " ", " ", " ");
      display.showMeasure(1, strength_buff);
      display.showMeasure(2, range_buff);
    }
    waitForUser(" ", showMeasure);
    //count = 0;
    Serial.print("[CAL]\tMEDICION: ");Serial.println(strength);
    strength = 0;
  }else{
    /*
    disposable = measure.strength();
    Serial.print("[CAL]\tFUERZA: ");Serial.print(disposable);
    Serial.print("|\tSEÑAL: ");Serial.print(measure.raw());Serial.println("|\tFuerza insuficiente");
    if(disposable < 0)disposable = 0;
    sprintf(disposable_buff, "0 g"); // Se fija un cero para que se muestre por pantalla
    display.showMeasure(disposable_buff);
    */
    display.showMessage("Listo para medir");
  }
  //}else{
  if(flag){
    display.showMessage("Finalizando medicion...");
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
		//display.showImage(PUSH);
  //}
}

/*
  Se encapsulan funciones para mejorar la lectura del codigo.
*/
/*
void switcher(){
  button = !button;
  Serial.print("[CAL]\tEstado del botón cambiado: ");Serial.println(button);
  while(digitalRead(TOUCH) == HIGH);
}
*/

bool minimumForce(int threshold){
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

unsigned int counter(){
  unsigned int number;
  //EEPROM.begin(memorySize);
  EEPROM.get(memoryLocation[1], number);
	number++;
  //EEPROM.put(memoryLocation[1],number);
  //EEPROM.commit();
  EEPROM.update(memoryLocation[1],number);
  //EEPROM.end();
  return number;
}
