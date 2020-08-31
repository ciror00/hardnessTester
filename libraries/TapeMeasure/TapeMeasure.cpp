#include "TapeMeasure.h"

bool TapeMeasure::begin(){
	;
}

int TapeMeasure::getSize(int iteration){
	this->spear = sonar.ping_median(iteration);
	return this->spear;
}

int TapeMeasure::takeSize(int iteration){
	int count = 5;
	float samples = 0;
	float range;
	unsigned int result;
	while(count >= 0){
		range = sonar.ping_median(iteration, this->spear);
		if(range <= this->spear && range >= this->dig){
			samples += range;
			count--;
		}
	}
	result = this->spear - (samples / 5.0);
	this->dig = result;
	return this->dig;
}

void TapeMeasure::reset(){
	this->dig = 0;
}