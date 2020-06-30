#ifndef __RECORDER__h__
#define __RECORDER__h__

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
  bool clock;
	bool setting;
  char buffer[50];
  //sprintf(buffer, "Esto es una prueba de enteros %d, largos %ld y cadenas %s", num, lnum, str);
  //Serial.print(buffer);
	public:
		Recorder() = default;
    bool begin(const int cs);
    bool setTitles(int numb, ...);
    bool saveRegistry(int numb, ...);
		~Recorder() = default;
};
#endif
