#include "LoadCell.h"

void LoadCell::begin(const int dout, const int pd_sck, const byte gain){
  this->doubleEnded.begin(dout, pd_sck);
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
  return this->doubleEnded.read();
}

void LoadCell::calibrate(long patternWeight, int samples, byte iteration){
  this->doubleEnded.set_scale();  // Se setea una escala por defecto (1)
  this->doubleEnded.tare(samples); // Se toma el peso actual como Tara
  for (byte i = 0; i < iteration; i++) {
    readValue = readValue + this->doubleEnded.get_value(samples); // Facto de escala
  };
  layover = readValue / patternWeight;
  while(t = 5000){t--;};
  // Se establece una escala con el factor calculado
  this->doubleEnded.set_scale(layover);  // Se setea una escala por defecto (1)
  this->doubleEnded.tare(samples); // Se toma el peso actual como Tara
  while(t = 5000){t--;};
  this->doubleEnded.get_units(samples); // Se muestra
}
