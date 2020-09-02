#include "TapeMeasure.h"

bool TapeMeasure::begin(){
	;
}

int TapeMeasure::_getSize(int iteration){
	int period = this->sonar.ping_median(iteration);
	return this->sonar.convert_cm(period);
}

int TapeMeasure::calibrateLance(int iteration){
	this->spear = this->_getSize(iteration);
	return this->spear;
}

int TapeMeasure::takeSize(int iteration){
	if(this->spear <= 0)return 0;
	int range, result;
	range = this->_getSize(iteration);
	result = this->spear - range;
	if(result >= this->dig){
		this->dig = result;
	}
	return this->dig;
}

void TapeMeasure::reset(){
	this->dig = 0;
}