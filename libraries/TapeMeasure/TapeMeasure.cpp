#include "TapeMeasure.h"

bool TapeMeasure::begin(){
	;
}

int TapeMeasure::_getSize(int iteration){
	int period = sonar.ping_median(iteration);
	return sonar.convert_cm(period);
}

int TapeMeasure::calibrateLance(int iteration){
	this->spear = this->_getSize(iteration);
	return this->spear;
}

int TapeMeasure::takeSize(int iteration){
	int range, result;
	range = this->_getSize(iteration);
	result = this->spear - range;
	this->dig = result;
	return this->dig;
}

void TapeMeasure::reset(){
	this->dig = 0;
}