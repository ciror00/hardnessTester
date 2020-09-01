#include "TapeMeasure.h"

bool TapeMeasure::begin(){
	;
}

int TapeMeasure::getSize(int iteration){
	int period = sonar.ping_median(iteration);
	this->spear = sonar.convert_cm(period);
	return this->spear;
}

int TapeMeasure::takeSize(){
	int count = 5;
	int range, dummy;
	unsigned int result;
	unsigned int samples = 0;
	while(count >= 0){
		//range = sonar.ping_cm();
		dummy = sonar.ping_median();
		range = sonar.convert_cm(dummy);
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