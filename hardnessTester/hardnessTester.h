#include <EEPROM.h>

#include "LoadCell.h"
#include "Visualizer.h"
#include "Recorder.h"
#include "DataHandler.h"

#define FIRMWARE 0.640

#define SCK_CELL 0 // D3
#define DT_CELL 2 // D4
#define CS 15 // D8
#define TOUCH 16  // D0

// Declaraciones de la configuracion, editables
const long sensibility = 40; // Valor minimo de fuerza para tomar un registro [gr]
const byte stabilizer = 2; // Cantidad de muestras antes de devolver una medicion
const int showMeasure = 1000; // Tiempo que se muestra la medicion por pantalla
const int waitConfiguration = 6000; // Tiempo de espera para entrar en modo configuracion
const int trick = 0; // Cantidad de digitos a truncar en el valor de medicion
//const long elasticLimit = 13000; // Valor maximo relativo soportado por el equipo [gr]

// Otras declaraciones
bool conf = false;
bool flag = false;
bool button = false;
//int count = 0L;
int fruit = 0;
byte memorySize = 64;
byte memoryLocation[] = {0, memorySize/2};
float patternWeight = 0;
float manualScale = 0;
float strength = 0;

bool sdModule;
//float averange;
float disposable;
float maximum, minimum;
long lot;

char lot_buff[10];
char count_buff[10];
char strength_buff[10];
char average_buff[10];
char force_buff[10];
char percentages_buff[10];
char max_buff[10];
char min_buff[10];
char disposable_buff[10];
