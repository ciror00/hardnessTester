#include <EEPROM.h>

#include "LoadCell.h"
#include "Visualizer.h"
#include "Recorder.h"
#include "DataHandler.h"

#define SCK_CELL 0 // D3
#define DT_CELL 2 // D4
#define CS 15 // D8
#define TOUCH 16  // D0
/*
 * Configuración de pines
 *
 * D0 = 16
 * D1 = 5
 * D2 = 4
 * D3 = 0
 * D4 = 2
 * D5 = 14
 * D6 = 12
 * D7 = 13
 * D8 = 15
 *
*/

bool conf = false;
byte button = 0;
int sensibility = 100;
long strength, average;
float scale, patternWeight;

char strength_buff[50];
char average_buff[50];
