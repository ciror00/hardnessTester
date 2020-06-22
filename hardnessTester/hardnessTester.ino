#include "hardnessTester.h"

LoadCell forceTransducer;

void setup(){
  Serial.begin(115200);
  forceTransducer.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

void loop(){
  Serial.println("Hola Mundo");
}
