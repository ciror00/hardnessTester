#include "ground.h"

LiteVisualizer display;
LoadCell measure;
Recorder recorder;
DataHandler forceAnalyzer;
TapeMeasure tapeMeasure;

void setup(){
  Serial.begin(115200);
  serial_gps.begin(9600);
  Serial.println("\n\"HARDNESS TESTER (ground)\"\nFirmware: "+ (String)FIRMWARE + \
                "\t| Environment: " + (String)ARDUINO + "\t| Compiler: "+ (String)__VERSION__);
  display.begin();
  display.switcher(true);
  if(LOGS){
    display.showMessage("DEBBUGING MODE", " ", "");
  }else{
    display.showMessage("  COPAINS S.R.L.", " ", "  Iniciando...");
  }
  measure.begin(DT_CELL, SCK_CELL);
  recorder.begin(CS);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // Configuacion de modulo
  delay(2000);
  display.showMessage("Mantener en vertical", "Calibrando equipo...", " ");
  if(recorder.clock()){
    Serial.println("[OK]\tRTC operativo");
    if(recorder.card()){
		  sdModule = true;
	  Serial.println("[OK]\tSD");
    }else{
			sdModule = false;
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
    EEPROM.put(memoryLocation[0], manualScale);
		EEPROM.put(memoryLocation[1], 1);
		Serial.println("[MJS]\tMEDICION No: 1 (contador reiniciado)");
		Serial.println("\n[MJS]\tReinicia el equipo manualmente...");
    delay(99999);
  }else if(scaleCalculation == 0){
		Serial.println("\n[MSJ]\tCalibración automática cancelada.");
    Serial.println("[MSJ]\tCalibración por defecto.");
    manualScale = scale;
    EEPROM.put(memoryLocation[1], 1);
		Serial.println("[MJS]\tMEDICION No: 1 (contador reiniciado)");
    EEPROM.put(memoryLocation[0], manualScale);
    Serial.print("[CAL]\tFACTOR: "); Serial.println(manualScale);
  }else if(scaleCalculation == -1){
		Serial.println("\n[MSJ]\tCargando configuracion guardada.");
    EEPROM.get(memoryLocation[0], manualScale);
    Serial.print("[MSJ]\tFACTOR: "); Serial.println(manualScale);
    measure.manualSetup(manualScale);
    EEPROM.get(memoryLocation[1], lot);
    Serial.print("[CAL]\tMEDICION No: ");Serial.println(lot);
	}

  Serial.println("[MSJ]\tConfiguranndo GPS");
  if(connecting(4000)){
    gpsModule = true;
    gps.stats(&chars, &sentences, &failed);
    Serial.print("> Estaditicas: \n> char: "); Serial.print(chars); Serial.print("| sentences: ");
    Serial.print(sentences); Serial.print(" failed: "); Serial.println(failed);
    Serial.print(">Lat: "); Serial.print(lat);Serial.print("\t|Lon: "); Serial.println(lon);
    Serial.print(">Fecha: "); Serial.print(year); Serial.print(month); Serial.println(day);
    Serial.print(">Hora: "); Serial.print(hour); Serial.print(minute); Serial.println(second);
    if(year!=0){
      Serial.println("[MSJ]\tSincronizacion RTC (GMT -3)");
      recorder.setDate(year,month,day,hour-3,minute);
    }else{
      Serial.println("[ERROR]\tSincronizacion RTC fallida");
      recorder.setDate();
    }
  }else{
    Serial.println("[ERROR]\tFalla de comunicación con GPS");
    Serial.println("[MSJ]\tFecha y hora por defecto.");
  }
  dtostrf(flat,2,6,lat_buff);
  dtostrf(flon,2,6,lon_buff);
  //sprintf(lat_buff, "%ld", lat);
  //sprintf(lon_buff, "%ld", lon);

  if(LOGS)recorder.logger(4, "LAT: " , lat_buff, "LON: ", lon_buff);
  recorder.showTime();

	// Se agregan los titulos de archivos, que viene despues de los "Fecha" y "Hora" (titulos por defecto)
  // {"Latitud", "Longitud", "Medicion", "Fuerza [KG]", "Distancia [CM]", "Distancia total", "Fuerza Promedio", "Fuerza Maxima", "Distancia Fuerza Maxima"};
  headers = recorder.setTitles(9, titles[0], titles[1], titles[2], titles[3], titles[4], titles[5], titles[6], titles[7], titles[8]);
  sprintf(lot_buff, "%d", lot);
 	recorder.saveRegistry(9, lat_buff, lon_buff, lot_buff, " ", " ", " ", " ", " ", " "); // Ejecucion estetica, no funcional
  // Se configurar los pines usando métodos de Arduino
  Serial.println("[MSJ]\tMidiendo jabalina.");
  spear = tapeMeasure.calibrateLance(15);
  if(spear > 0){
    Serial.print("[CAL]\tLargo de lanza medida: ");Serial.println(spear);
  }else{
    Serial.println("[ERROR]\tEn medicion. Configurando lanza por defecto");
    Serial.print("[CAL]\tLargo de lanza: ");Serial.println(spear);
  }
  Serial.println("[MSJ]\tConfiguracion terminada. Listo para medir.");
  display.home(sdModule, gpsModule);
  delay(2500);
}

void loop(){
  if(!headers)headers = recorder.setTitles(9, titles[0], titles[1], titles[2], titles[3], titles[4], titles[5], titles[6], titles[7], titles[8]);
  if(minimumForce(sensibility)){ // Primero descarta que no sea ruido de la lanza
    if(close){
      Serial.print("[CAL]\tMEDICION No: ");Serial.println(lot);
      close = false;
      sprintf(lot_buff, "%04d", lot);
      recorder.saveRegistry(9, lat_buff, lon_buff, lot_buff, " ", " ", " ", " ", " ", " "); // Ejecucion estetica, no funcional
    }
    flag = true;
    display.reset();
    while(measure.strength() > sensibility){ // Hace un bucle, mientras se ejerza mas fuerza que la minima
      // Medicion de fuerza
      strength = measure.strengthAverage(stabilizer);
      if(strength == 0){ // filtro para evitar mediciones por histeresis
        flag = false;
        display.home(sdModule, gpsModule);
        break;
      }
      Serial.print("[CAL]\tFUERZA: ");Serial.print(strength);
      // Medicion de distancia
      depth = tapeMeasure.makeAWell(15, tolerance);
      Serial.print("|\tPROFUNDIDAD: ");Serial.print(depth);
      // Punto de maxima fuerza
      if(specimen <= strength){
        specimen = strength;
        point = depth; 
      }
      // Muestra de informacion por pantalla
      dtostrf(strength,2,2,strength_buff);
      sprintf(depth_buff, "%04d", depth);
      display.showHeader(sdModule, gpsModule);
      display.showMeasure(1, "F:", strength_buff, "[Kg]", false);
      display.showMeasure(2, "D:", depth_buff, "[cm]", false);
      // Carga de datos en memoria para calculos posteriores
      forceAnalyzer.preLoad(strength);
      if(!recorder.card()){
        Serial.println("[ERROR]\tSD mal configurada");
        sdModule = false;
      }else{
        Serial.println("[MJS]\tMedicion guarda en SD");
        sdModule = true;
      // {"Latitud", "Longitud", "Medicion", "Fuerza [KG]", "Distancia [CM]", "Distancia total", "Fuerza Promedio", "Fuerza Maxima", "Distancia Fuerza Maxima"};
        recorder.saveRegistryTimeless(9, " ", " ", " ", strength_buff, depth_buff, " ", " ", " ", " ");
      }
      strength = 0;
      top = depth;
      depth = 0;
    }
  }else{
    token = witness(updateTime*1000*60); // 1K milisegundos = 1 segundo * 60 = 1 minutos
    if(token > update){
      display.showMessage(" ", "Sincronizando", " ");
      Serial.println("[MSJ]\tActualizando GPS...");
      update = token;
      gpsModule = (connecting(4000)) ? true : false;
      dtostrf(flat,2,6,lat_buff);
      dtostrf(flon,2,6,lon_buff);
      //sprintf(lat_buff, "%ld", lat);
      //sprintf(lon_buff, "%ld", lon);
      sdModule = (recorder.card()) ? true : false;
      display.home(sdModule, gpsModule);
      if(LOGS)recorder.logger(4, "LAT: " , lat_buff, "LON: ", lon_buff);
    }
  }
  if(flag){
    display.showMessage(" ", "Procesando...", " ");
    flag = false;
    dtostrf(forceAnalyzer.maximum(),2,2,max_buff);
    dtostrf(forceAnalyzer.average(),2,2,average_buff);
    sprintf(range_buff, "%02d", top);
    sprintf(point_buff, "%02d", point);
    if(!recorder.card()){
      Serial.println("[ERROR]\tSD no reconocida");
      sdModule = false;
    }else{
      Serial.println("[MJS]\tResumen guardado en SD");
    // {"Latitud", "Longitud", "Medicion", "Fuerza [KG]", "Distancia [CM]", "Distancia total", "Fuerza Promedio", "Fuerza Maxima", "Distancia Fuerza Maxima"};
      recorder.saveRegistryTimeless(9," ", " ", " ", " ", " ", range_buff, average_buff, max_buff, point_buff);
      sdModule = true;
    }
    Serial.println("[MJS]\tResumen de datos calculados");
    Serial.print("> F. PROMEDIO: ");Serial.println(average_buff);
    Serial.print("> F. MAXIMA: ");Serial.println(max_buff);
    Serial.print("> PROFUNDIDAD: ");Serial.println(range_buff);
    Serial.print("> PROF. de F. MAXIMA: ");Serial.println(point_buff);
    display.detail(sdModule, gpsModule, lot_buff);
    // Pantalla de resumen. Se muestra: Fuerza promedio, Fuerza maxima, Distancia Maxima, Punto de mayor fuerza
    display.summary(average_buff, max_buff, range_buff, point_buff);
    forceAnalyzer.reset();
    top = 0;
    tapeMeasure.reset();
    delay(5000);
    lot = counter();
    close = true;
    specimen = 0;
    point = 0;
    display.home(sdModule, gpsModule);
  }
}

/*
  Se encapsulan funciones para mejorar la lectura del codigo.
*/

bool minimumForce(int threshold){
  int t = 0;
  float s = measure.strength();
  while(s >= threshold && t < stabilizer){
    t++;
    s = measure.strength();
    Serial.print("> SENSOR: ");Serial.print(s);Serial.print("\t| MUESTRAS: ");Serial.println(t);
  }
	bool mf = (t >= stabilizer) ? true : false;
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

long witness(long tht){
  if(tht == 0)return 0;
  unsigned long time_now = millis();
  long ring = round(time_now / tht);
  return ring;
}

unsigned int counter(){
  unsigned int number;
  EEPROM.get(memoryLocation[1], number);
	number++;
  EEPROM.put(memoryLocation[1],number);
  return number;
}

bool connecting(int wait){
  bool satelite = false;
  Serial.print(">Sincronizando");
  for(unsigned long start = millis(); millis() - start < wait;){
    while (serial_gps.available()) {
      // Se realiza un "ping" a modulo GSP
      char c = serial_gps.read();
      if (gps.encode(c)) {
        Serial.print(c);
        satelite = true;
      }
    }
  }
  if(satelite){
    //gps.f_get_position(&flat, &flon, &age); // Obtengo posicion
    gps.get_position(&lat, &lon, &age);
flat = lat/100000;
flon = lon/100000;
    gps.crack_datetime(&year, &month, &day, \
      &hour, &minute, &second, &hundredths, &age); // Obtengo fecha y hora
    Serial.println("... OK");
    return true;
  }else{
    Serial.println("... TIME OUT");
    return false;
  }
}
