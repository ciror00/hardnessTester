#ifndef __RECORDER__h__
#define __RECORDER__h__
#define PRINT(...) Serial.print(__VA_ARGS__)

#include <Arduino.h>
#include <Wire.h>
#include <stdarg.h>
#include <RTClib.h>
#include <SPI.h>
#include <SD.h>

class Recorder{
  RTC_DS3231 rtc;
  DateTime date;

  File registry;
  int cs;
  bool clocker;
	bool setting;
  char buffer[50];

	public:
		Recorder() = default;
    bool begin(const int cs);
    bool clock(int gmt = 0);
    bool card();
    void showTime();
    void setDate(int y=2020, int m=1, int d=1, int h=0, int x=0);
    void setUTC(int sinc);
    bool setTitles(int numb, ...);
    bool saveRegistry(int numb, ...);
    bool saveRegistryTimeless(int numb, ...);
		~Recorder() = default;
};
#endif
