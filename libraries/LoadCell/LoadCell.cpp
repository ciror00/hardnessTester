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
  constrain(currentValue, 40, 100);
  return currentValue;
}

// Igual que strength() pero devuelve el promedio
long LoadCell::strengthAverage(int amount){
  currentValue = this->doubleEnded.get_units(amount);
  constrain(currentValue, 40, 100);
  return currentValue;
}

long LoadCell::raw(){
  crudeValue = this->doubleEnded.read();
  return crudeValue;
}

float LoadCell::calibrate(float patternWeight, int samples, int iteration){
  PRINT("\n> Modo calibracion activado.");
  if(patternWeight == 0){
    PRINT("\n> Peso incorrecto.");
    return 1;
  }
  float layover, censusWeight, calculationWeight;
  PRINT("\n> Señal: ");PRINT(this->doubleEnded.read());PRINT("\n");
  this->doubleEnded.set_scale();
  this->doubleEnded.tare(20);

  PRINT("> Colocar el peso a medir");
  int wait = 5;
  while(wait >= 0){
    PRINT("...");PRINT(wait);
    wait--;
    delay(1000);
  }

  int i;
  for (i = 0L; i < samples; i++) {
    censusWeight = this->doubleEnded.get_value(10);
    PRINT("\n> Lectura: ");
    PRINT(censusWeight);PRINT("");
    calculationWeight += censusWeight;
    delay(100);
  }
  PRINT("\n> Mediciones: ");PRINT(i);
  calculationWeight = calculationWeight / i;
  PRINT("\n> Promedio: ");PRINT(calculationWeight);
  PRINT("\n> Peso: ");PRINT(patternWeight);
  layover = calculationWeight / patternWeight;
  PRINT("\n> Escala (r): ");PRINT(layover);
  return layover;
}

void LoadCell::manualSetup(float layover){
  this->doubleEnded.set_scale(layover);  // Se setea una escala por defecto (1)
  this->doubleEnded.tare(10); // Se toma el peso actual como Tara
}
