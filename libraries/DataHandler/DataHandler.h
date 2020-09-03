#ifndef __DATAHANDLER__h__
#define __DATAHANDLER__h__
#include <Arduino.h>

class DataHandler{
  float list[400];
  int pointer = 0;


  public:
    DataHandler() = default;
    void begin(const int reserved = 50);
    void preLoad(float sampling);
    int getPointer();
    float average();
    float maximum();
    float minimum();
    float percentages(long max_value = 1, long current_value = 1);
    void reset();
    ~DataHandler() = default;
};
#endif
