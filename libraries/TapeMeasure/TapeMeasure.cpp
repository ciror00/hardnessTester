#include "TapeMeasure.h"

bool TapeMeasure::begin(){
	;
}

int TapeMeasure::getSize(int iteration){
	int period = sonar.ping_cm(iteration);
	this->spear = sonar.convert_cm(period);
	return this->spear;
}

int TapeMeasure::takeSize(){
	int count = 5;
	float samples = 0;
	float range;
	unsigned int result;
	while(count >= 0){
		range = sonar.ping_cm();
		if(range <= this->spear && range >= this->dig){
			samples += range;
			count--;
		}
	}
	result = this->spear - (samples / 5.0);
	//this->dig = sonar.convert_cm(result);
	this->dig = result;
	return this->dig;
}

void TapeMeasure::reset(){
	this->dig = 0;
}