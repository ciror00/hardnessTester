#include <EEPROM.h>
#include <TinyGPS.h>
//#include <SoftwareSerial.h>

#include "LoadCell.h"
#include "LiteVisualizer.h"
#include "Recorder.h"
#include "DataHandler.h"
#include "TapeMeasure.h"

#include "settings.h"

#define FIRMWARE 1.0

#define TRIG 4
#define ECHO 5

#define TEMP 14

#define RXD 16
#define TXD 17

//#define CLK 18
//#define MISO 19
//#define MOSI 23
#define CS 27

#define DT_CELL 25
#define SCK_CELL 26

// Invocacion de funciones
//SoftwareSerial serial_gps(TXD, RXD);
TinyGPS gps;

// Declaraciones internas del programa
float flat, flon;
long lat, lon;
unsigned long age, date, timer, chars;
unsigned short sentences, failed;
int year = 0;
byte month, day, hour, minute, second, hundredths;
int fixedPoint[] = {2, 6};

bool conf = false;
bool flag = false;
bool headers = false;
bool closer = false;
bool geo = false;
bool card = false;

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

unsigned long token = 0;
unsigned long updater = 0;

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
