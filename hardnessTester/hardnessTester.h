#include <EEPROM.h>

#include "LoadCell.h"
#include "Visualizer.h"
#include "Recorder.h"
#include "DataHandler.h"
#include "settings.h"

#define FIRMWARE 0.572

#define SCK_CELL 0 // D3
#define DT_CELL 2 // D4
#define CS 15 // D8
#define TOUCH 16  // D0

bool conf = false;
bool flag = false;
bool button = false;
int count = 0;
int fruit = 0;
byte memorySize = 64;
byte memoryLocation[] = {0, memorySize/2};
float wait = 0;
float patternWeight = 0;
float manualScale = 0;

bool sdModule;
float averange;
long strength;
unsigned long sum;
long lot;

char lot_buff[10];
char count_buff[10];
char strength_buff[10];
char percentages_buff[10];

char max_buff[10];
char min_buff[10];
char average_buff[10];
