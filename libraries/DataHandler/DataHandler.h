#ifndef __DATAHANDLER__h__
#define __DATAHANDLER__h__
#include <Arduino.h>

class DataHandler{
  float list[50];
  //long sampling[50];
  int pointer = 0;

  public:
    DataHandler() = default;
    void begin(const int reserved);
    void preLoad(float sampling);
    float average();
    float maximum();
    float minimum();
    float percentages(long max_value = 1, long current_value = 1);
    void reset();
    int incrementor();
    ~DataHandler() = default;
};
#endif
