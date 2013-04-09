#include <cmath>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include "source.h"
#include "constant.h"

double single_frequency::set(const double magnitude, const double frequency){
	assert((magnitude > 0) and (frequency > 0));
	f = frequency;
	mag = magnitude;
	return 2*frequency;
}

double single_frequency::get(const double time){
	return mag*sin(2*pi*f*time);
}

double gaussian::set(const double _mean, const double frequency){
	assert((_mean > 0) and (frequency > 0));
	//mean = _mean;
	var = 0.5/(pi*frequency);
	mean = 4*var;
	return 2*frequency;
}

double gaussian::get(const double time){
	return exp(-0.5*pow((time-mean)/var, 2.0))/(sqrt(2*pi)*var);
}

double deri_gaussian::set(const double _mean, const double frequency){
	assert((_mean > 0) and (frequency > 0));
	//mean = _mean;
	var = 0.5/(pi*frequency);
	mean = 4*var;
	return 2*frequency;
}

double deri_gaussian::get(const double time){
	return -4*(time-mean)*exp(-0.5*pow((time-mean)/var, 2.0))/(sqrt(2*pi)*var);
}
