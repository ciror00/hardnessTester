#include <EEPROM.h>

#include "LoadCell.h"
#include "Visualizer.h"
#include "Recorder.h"
#include "DataHandler.h"
#include "settings.h"

#define FIRMWARE 0.38

bool conf = false;
bool flag = false;
byte button = 0;
int count = 0;
int fruit = 0;

float manualScale, patternWeight, averange;
long strength;
unsigned long sum;

char lot[5];
char count_buff[10];
char strength_buff[10];
char percentages_buff[10];

char max_buff[10];
char min_buff[10];
char average_buff[10];
