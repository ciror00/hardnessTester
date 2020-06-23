#include "LoadCell.h"

void LoadCell::begin(const byte dout, const byte pd_sck, const byte gain){
  this->dout = dout;
  this->pd_sck = pd_sck;
  this->gain = gain;
  this->doubleEnded.begin(dout, pd_sck, gain);
}

// Devuelve el valor actual restado del peso de tara y dividido por la escala
long LoadCell::strength(){
  currentValue = this->doubleEnded.get_units();
  return currentValue;
}

// Igual que strength() pero devuelve el promedio
long LoadCell::strengthAverage(int amount){
  currentValue = this->doubleEnded.get_units(amount);
  return currentValue;
}

long LoadCell::raw(){
  crudeValue = this->doubleEnded.read();
  return crudeValue;
}

void LoadCell::calibrate(long patternWeight, int samples, int iteration){
  PRINT("Modo calibracion.\nMantengan colgado para medir el peso.");
  this->doubleEnded.set_scale();  // Se setea una escala por defecto (1)
  this->doubleEnded.tare(samples); // Se toma el peso actual como Tara
  for (byte i = 0; i < iteration; i++) {
    // Se toman las muestras para determinar el peso aproximado del equipo
    readValue = readValue + this->doubleEnded.get_value(samples);
  };
  layover = readValue / patternWeight; // Se calcula una factor de escala PESO MEDIDO / PESO REAL
  PRINT("Factor calculado. Apoyo el equipo para terminar la calibracion...\n");
  while(t = 5000){t--;};
  PRINT("Terminando calibracion. Espera un momento...\n");
  // Se establece una escala con el factor calculado
  this->doubleEnded.set_scale(layover);  // Se setea una escala por defecto (1)
  this->doubleEnded.tare(samples); // Se toma el peso actual como Tara
  while(t = 5000){t--;};
  //this->doubleEnded.get_units(samples);
  PRINT("Equipo calibrado.\n");
}
