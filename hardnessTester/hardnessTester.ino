#include "hardnessTester.h"

LoadCell measure;

long n, m;

void setup(){
  Serial.begin(115200);
  Serial.println("\nInicio.");
  measure.begin(DOUT, SCK);
  Serial.print("Crudo: ");
  Serial.println(measure.raw());
  measure.calibrate(3, 10, 10);
}

void loop(){
  Serial.print("Crudo: ");
  Serial.println(measure.raw());
  Serial.print("Fuerza: ");
  Serial.println(measure.strength());
  Serial.print("Fuerza promedio: ");
  Serial.println(measure.strengthAverage(10));
}
