#include <EEPROM.h>

#include "LoadCell.h"
#include "Visualizer.h"
#include "Recorder.h"
#include "DataHandler.h"

#define FIRMWARE 0.58

#define SCK_CELL 0 // D3
#define DT_CELL 2 // D4
#define CS 15 // D8
#define TOUCH 16  // D0

const int sensibility = 100; // Valor minimo de señal para no descargar la muestra
const byte stabilizer = 2; // Cantidad de muestras antes de devolver una medicion
const int between = 10000; // Tiempo de espera para entrar en modo configuracion

// Otras declaraciones
bool conf = false;
bool flag = false;
bool button = false;
int count = 0L;
int fruit = 0;
byte memorySize = 64;
byte memoryLocation[] = {0, memorySize/2};
float patternWeight = 0;
float manualScale = 0;

bool sdModule;
float averange;
float strength;
float sum, maximum, minimum;
long lot;

char lot_buff[10];
char count_buff[10];
char strength_buff[10];
char percentages_buff[10];

char max_buff[10];
char min_buff[10];
char average_buff[10];
