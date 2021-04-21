#include "TapeMeasure.h"

void TapeMeasure::begin(int trig, int echo, int limit){
	// Dummy
	int t = trig;
  int e = echo;
	//this->sonar = NewPing(trig, echo, 100);
}

unsigned int TapeMeasure::_getSize(int iteration){
	unsigned int period = 0;
	period = this->sonar.ping_median(iteration);
	return this->sonar.convert_cm(period);
}

int TapeMeasure::calibrateLance(int iteration){
	this->spear = this->_getSize(iteration);
	return this->spear;
}

int TapeMeasure::makeAWell(int iteration, int tolerance){
	if(this->spear <= 0)return 0;
	int range = 0;
	int result = 0;
	int shifting = 0;
	//int range, result, shifting;
	range = this->_getSize(iteration);
	result = this->spear - range;
	shifting = result - this->dig;
	if(result > this->dig && shifting > tolerance){
		this->dig = result;
	}
	return this->dig;
}

void TapeMeasure::reset(){
	this->dig = 0;
}
