#include "Recorder.h"

bool Recorder::begin(const int cs){
	Wire.begin();
	this->cs = cs;
	this->clocker = false;
	this->setting = false;
}

bool Recorder::clock(int gmt){
	if (!this->rtc.begin()) {
    return this->clocker;
	}else{
		this->clocker = true;
	}
	if (this->rtc.lostPower()) {
		// Fijar a fecha y hora de compilacion
		this->rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   }
	 return this->clocker;
}

bool Recorder::card(){
 if (!SD.begin(this->cs)) {
	 return this->setting;
 }
 this->setting = true;
 return this->setting;
}

void Recorder::setDate(int y, int m, int d, int h, int x){
	PRINT("> Ajute de hora por defecto\n");
	this->rtc.adjust(DateTime(y, m, d, h, x, 0));
}

void Recorder::setUTC(int sinc){
	PRINT("> Correccion de hora UTC\n");
	this->date = this->rtc.now();
	this->rtc.adjust(DateTime(this->date.year(), this->date.month(), this->date.day(),\
	this->date.hour()+sinc, this->date.minute(), this->date.second()));
}

void Recorder::showTime(){
	char t[32];
	this->date = this->rtc.now();
	sprintf(t, "> %02d:%02d:%02d %02d/%02d/%04d \n",  \
		this->date.hour(), this->date.minute(), this->date.second(), \
		this->date.day(), this->date.month(), this->date.year());
		PRINT(t);
}

/*
		Se crea un método para configurar los titulos del CSV.
		Por defecto, se crean los titulos "Fecha" y "Hora" como primeras columnas.
		Se pasa por parametro la cantidad de titulos, despues de los por defecto. Tambien
	se pasan los todos los titulos separados por coma.

*/

bool Recorder::setTitles(int numb, ...){
	this->date = this->rtc.now();
	sprintf(buffer, "%04d%02d%02d.CSV", this->date.year(), this->date.month(), this->date.day());
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
	sprintf(buffer, "%04d%02d%02d.CSV", this->date.year(), this->date.month(), this->date.day());
	// Primero se chequea que el archivo exista
	if(SD.exists(buffer)){
		this->registry = SD.open(buffer, FILE_WRITE);
		String data;
		sprintf(buffer, "%02d/%02d/%04d;", this->date.day(), this->date.month(), this->date.year());
		data += buffer;
		sprintf(buffer, "%02d:%02d:%02d;", this->date.hour(), this->date.minute(), this->date.second());
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
