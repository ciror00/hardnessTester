#include "Recorder.h"

bool Recorder::begin(const int cs){
	Wire.begin();
	// Se inicia el RTC
	if (!this->rtc.begin()) {
		this->setting = false;
    return this->setting;
	}else{
		this->clock = true;
	}
	if (this->rtc.lostPower()) {
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Fijar a fecha y hora de compilacion
   }
	 // Se inicia la SD
	if (!SD.begin(cs)) {
    this->setting = false;
    return this->setting;
  }
  this->setting = true;
  return this->setting;
}

/*
		Se crea un método para configurar los titulos del CSV.
		Por defecto, se crean los titulos "Fecha" y "Hora" como primeras columnas.
		Se pasa por parametro la cantidad de titulos, despues de los por defecto. Tambien
	se pasan los todos los titulos separados por coma.

*/
bool Recorder::setTitles(int numb, ...){
	this->date = this->rtc.now();
	sprintf(buffer, "%d%d%d.CSV", this->date.year(), this->date.month(), this->date.day());
	// Primero se chequea que el archivo NO exista
	if(SD.exists(buffer) != true){
		this->registry = SD.open(buffer, FILE_WRITE);
		// Se recorren todos los titulos requeridos y se escriben en el archivo
		String titles;
		titles += "Fecha;Hora;";
		va_list ap;
		va_start(ap, numb);
		for(byte i = 0; i < numb; i++) {
			titles += va_arg(ap, const char *);
			titles += ";";
		}
		va_end(ap);
		this->registry.print(titles);
		this->registry.println();
		this->registry.close();
		return true;
	}else{
		// Si el archvo existe, no se pueden pisar los titulos
		return false;
	}
}

/*
		Se crea un método para agregar registros al CSV.
		Por defecto, se agregan los datos "Fecha" y "Hora" como primeras columnas.
		Se pasa por parametro la cantidad de datos a agregar. Tambien	se pasan los
	datos separados por coma.
		Tener en cuenta el orden de los titulos para incluir los registros.
*/
bool Recorder::saveRegistry(int numb, ...){
	this->date = this->rtc.now();
	sprintf(buffer, "%d%d%d.CSV", this->date.year(), this->date.month(), this->date.day());
	// Primero se chequea que el archivo exista
	if(SD.exists(buffer)){
		this->registry = SD.open(buffer, FILE_WRITE);
		String data;
		sprintf(buffer, "%d/%d/%d;", this->date.day(), this->date.month(), this->date.year());
		data += buffer;
		sprintf(buffer, "%d:%d:%d;", this->date.hour(), this->date.minute(), this->date.second());
		data += buffer;
		// Se recorren todos los datos que se quieren ingresar en el archivo
		va_list ap;
		va_start(ap, numb);
		for(byte i = 0; i < numb; i++) {
			data += va_arg(ap, const char *);
			data += ";";
		}
		va_end(ap);
		this->registry.print(data);
		this->registry.println();
		this->registry.close();
		return true;
	}else{
		// Si el archivo NO existe, primero se tiene que setear los titulos
		return false;
	}
}
