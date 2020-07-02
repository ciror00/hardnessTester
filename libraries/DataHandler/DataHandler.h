#ifndef __DATAHANDLER__h__
#define __DATAHANDLER__h__
#include <Arduino.h>

class DataHandler{
  long list[50];
  //long sampling[50];
  byte pointer = 0;

  public:
    DataHandler() = default;
    void begin(const int reserved);
    void preLoad(long sampling);
    float average();
    float maximum();
    float minimum();
    float percentages(long max_value, long current_value);
    void reset();
    int incrementor();
    ~DataHandler() = default;
};
#endif
