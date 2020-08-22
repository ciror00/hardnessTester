#include <EEPROM.h>
#include <NewPing.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

#include "LoadCell.h"
//#include "Visualizer.h"
#include "LiteVisualizer.h"
#include "Recorder.h"
#include "DataHandler.h"

#include "settings.h"

#define FIRMWARE 0.01

#define SCK_CELL A12
#define DT_CELL A11
#define CS 53
//#define TOUCH 
#define TRIG 8
#define ECHO 9 
#define RXD 10
#define TXD 11

// Invocacion de funciones
NewPing rule(TRIG, ECHO, 250);
SoftwareSerial serial_gps(TXD, RXD);
TinyGPS gps;

// Declaraciones internas del programa
unsigned long age;
float lat;
float lon;
int y = 0;
byte hour, m, d, min, seg, hundredths;

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
