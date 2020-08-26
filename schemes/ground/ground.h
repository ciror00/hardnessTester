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

#define FIRMWARE 0.23

#define SCK_CELL A12
#define DT_CELL A11
#define CS 53
#define TRIG 8
#define ECHO 9 
#define RXD 10
#define TXD 11

// Invocacion de funciones
NewPing rule(TRIG, ECHO, 250);
SoftwareSerial serial_gps(TXD, RXD);
TinyGPS gps;

// Declaraciones internas del programa
long lat, lon;
float flat, flon;
unsigned long age, date, time, chars;
unsigned short sentences, failed;
int year = 0;
byte month, day, hour, minute, second, hundredths;

bool conf = false;
bool flag = false;
//bool button = false;
bool headers = false;
bool close = false;
bool geo = false;
bool card = false;

//int fruit = 0;
byte memorySize = 64;
byte memoryLocation[] = {0, memorySize/2};
float patternWeight = 0;
float manualScale = 0;
float scaleCalculation = -1;
float strength = 0;
float range = 0; 
float depth = 0;
unsigned int lot = 1;

bool gpsModule = false;
bool sdModule;
float disposable, reducible;
float maximum, minimum;

long token = 0;
long update = 0;

char *titles[] = {"Medicion", "Distancia", "Latitud", "Longitud", "Dist. Max", "F. Maxima", "F. Minima", "F. Promedio"};

char lot_buff[10];
char count_buff[10];
char strength_buff[10];
char average_buff[10];
//char force_buff[10];
char range_buff[10];
char depth_buff[10];
char lat_buff[10];
char lon_buff[10];
//char location_buff[10];
char max_buff[10];
char min_buff[10];
char disposable_buff[10];
