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

float LoadCell::calibrate(long patternWeight, int samples, int iteration){
  if(patternWeight == 0){
    PRINT("\n> Peso del equipo mal ingresado.\n");
    return 1;
  }
  this->doubleEnded.set_scale();  // Se setea una escala por defecto (1)
  this->doubleEnded.tare(); // Se toma el peso actual como Tara
  int wait = 0;
  long layover = -999;
  long censusWeight = 0;
  PRINT("> Peso de referencia: ");PRINT(patternWeight);
  while(wait < 5000){wait--;};
  while(abs(patternWeight - censusWeight) > 10){
    this->doubleEnded.set_scale(layover);
    censusWeight = this->strengthAverage(samples);
    layover = layover + 10;
    PRINT("\n> Censo: ");PRINT(censusWeight);
    PRINT("|\tFactor: ");PRINT(layover);
  }
  return layover;
}

void LoadCell::manualSetup(float layover){
  this->doubleEnded.set_scale(layover);  // Se setea una escala por defecto (1)
  this->doubleEnded.tare(10); // Se toma el peso actual como Tara
}
