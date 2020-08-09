#include <EEPROM.h>

#include "LoadCell.h"
#include "Visualizer.h"
#include "Recorder.h"
#include "DataHandler.h"
#include "settings.h"

#define FIRMWARE 0.685

#define SCK_CELL 0 // D3
#define DT_CELL 2 // D4
#define CS 15 // D8
#define TOUCH 16  // D0

// Declaraciones internas del programa
bool conf = false;
bool flag = false;
bool button = false;
bool titles = false;
bool close = false;
int fruit = 0;
byte memorySize = 64;
byte memoryLocation[] = {0, memorySize/2};
float patternWeight = 0;
float manualScale = 0;
float scaleCalculation = -1;
float strength = 0;
unsigned int lot = 1;

bool sdModule;
float disposable;
float maximum, minimum;

char lot_buff[10];
char count_buff[10];
char strength_buff[10];
char average_buff[10];
char force_buff[10];
char percentages_buff[10];
char max_buff[10];
char min_buff[10];
char disposable_buff[10];
