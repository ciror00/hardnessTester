#include <EEPROM.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

#include "LoadCell.h"
#include "LiteVisualizer.h"
#include "Recorder.h"
#include "DataHandler.h"
#include "TapeMeasure.h"

#include "settings.h"

#define FIRMWARE 0.86

#define SCK_CELL A12
#define DT_CELL A11
#define CS 53
#define ECHO 9
#define TRIG 8
#define RXD 10
#define TXD 11

// Invocacion de funciones
SoftwareSerial serial_gps(TXD, RXD);
TinyGPS gps;

// Declaraciones internas del programa
float flat, flon;
long lat, lon;
unsigned long age, date, time, chars;
unsigned short sentences, failed;
int year = 0;
byte month, day, hour, minute, second, hundredths;

bool conf = false;
bool flag = false;
bool headers = false;
bool close = false;
bool geo = false;
bool card = false;
//bool zero = false;

byte memorySize = 64;
byte memoryLocation[] = {0, memorySize/2};
float patternWeight = 0;
float manualScale = 0;
float scaleCalculation = -1;
float strength = 0;
float specimen = 0;
int point = 0;
int spear = 50; 
int depth = 0;
int top = 0;
unsigned int lot = 1;

bool gpsModule = false;
bool sdModule;
float disposable, reducible;
float maximum, minimum;

long token = 0;
long update = 10;

char *titles[] = {"Latitud", "Longitud", "Medicion", "Fuerza [KG]", "Distancia [CM]", "Distancia total", "Fuerza Promedio", "Fuerza Maxima", "Distancia Fuerza Maxima"};

char float_buff[10];
char lot_buff[10];
char count_buff[10];
char strength_buff[10];
char average_buff[10];
char range_buff[10];
char point_buff[10];
char depth_buff[10];
char lat_buff[10];
char lon_buff[10];
char max_buff[10];
char min_buff[10];
char disposable_buff[10];
