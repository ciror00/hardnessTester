#include <EEPROM.h>

#include "LoadCell.h"
#include "Visualizer.h"
#include "Recorder.h"
#include "DataHandler.h"
#include "settings.h"

#define FIRMWARE 0.41

bool conf = false;
bool flag = false;
byte button = 0;
int count = 0;
int fruit = 0;
byte memorySize = 64;
byte memoryLocation[] = {0, memorySize/2};

float manualScale, patternWeight, averange;
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
