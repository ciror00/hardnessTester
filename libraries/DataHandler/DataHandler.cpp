#include "DataHandler.h"

void DataHandler::begin(const int reserved){
  ;
}

void DataHandler::preLoad(float sampling){
  this->list[this->pointer] = sampling;
  this->pointer++;
}

float DataHandler::average(){
  if(this->pointer == 0)return 0;
  float setSum, value;
  for (byte i = 0; i < this->pointer; i++){
    setSum += this->list[i];
  }
  value = setSum/this->pointer;
  return value;
}

float DataHandler::maximum(){
  if(this->pointer == 0)return 0;
  long bigger = 0;
  for (byte i = 0; i < this->pointer; i++){
    if (this->list[i] > bigger){
      bigger = this->list[i];
    }
  }
  return bigger;
}

float DataHandler::minimum(){
  if(this->pointer == 0)return 0;
  long smaller = this->list[0];
  for (byte i = 0; i < this->pointer; i++){
    if (this->list[i] < smaller){
      smaller = this->list[i];
    }
  }
  return smaller;
}

float DataHandler::percentages(long max_value, long current_value){
  return current_value*100/max_value;
}

void DataHandler::reset(){
  for (this->pointer; this->pointer > 0; this->pointer--) {
    this->list[this->pointer] = NULL;
  };
}

int DataHandler::incrementor(){
  return 0;
}
