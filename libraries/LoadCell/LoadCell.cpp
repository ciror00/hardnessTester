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
    PRINT("\nPeso del equipo mal ingresado.\n");
    return 1;
  }
  int wait = 0;
  PRINT("\n(Recuerde que el equipo debe estar apoyado para medir su propio peso)");
  this->doubleEnded.set_scale();  // Se setea una escala por defecto (1)
  this->doubleEnded.tare(samples); // Se toma el peso actual como Tara
  for (byte i = 0; i < iteration; i++) {
    // Se toman las muestras para determinar el peso aproximado del equipo
    readValue = readValue + this->doubleEnded.get_value(samples);
  };
  layover = readValue / patternWeight; // Se calcula una factor de escala PESO MEDIDO / PESO REAL
  PRINT("Factor calculado: ");PRINT(layover);
  //PRINT("\nMueva el equipo para que el sensor mida en vacío.\n");
  /*
  while(wait < 9000){wait++;};
  wait = 0;
  PRINT("Terminando calibracion. Espere un momento...\n");
  // Se establece una escala con el factor calculado
  this->doubleEnded.set_scale(layover);  // Se setea una escala por defecto (1)
  this->doubleEnded.tare(samples); // Se toma el peso actual como Tara
  while(wait < 9000){wait--;};
  //this->doubleEnded.get_units(samples);
  PRINT("Equipo calibrado.\n");
  */
  return layover;
}

void LoadCell::manualSetup(float layover){
  this->doubleEnded.set_scale(layover);  // Se setea una escala por defecto (1)
  this->doubleEnded.tare(10); // Se toma el peso actual como Tara
}
