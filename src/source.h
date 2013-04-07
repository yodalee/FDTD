#ifndef _SOURCE_H_
#define _SOURCE_H_

#include <cfloat>

//********************************************
// Class: source
// Description: this is the interface of source class
//  to generate voltage at specific time.
// Function: 
//  get() take the time and return the voltage value at that time.
//  set() take a few parameter the source need to initial the source.
//   and return the lowest sampling frequency
//********************************************
class source {
public:
	source (){};
	virtual double get(double){return 0;}
	virtual double set(double, double){return DBL_MAX;}
};

//********************************************
// Class: single_frequency
// Description: generate cos(2pi*frequency*time)
//  single frequency signal
//********************************************
class single_frequency : public source {
public:
	single_frequency (){};
	double get(const double time);
	double set(const double magnitude, const double frequency);
private:
	double mag;
	double f;
};

//********************************************
// Class: gaussian
// Description: generate source
//  (1/(sqrt(2*pi)*sigma)*exp^(-(t-m)^2/2sigma^2)))
//********************************************
class gaussian : public source {
public:
	gaussian (){};
	double get(const double time);
	double set(const double mag, const double freq);
private:
	double mean;
	double var;
};

//********************************************
// Class: degaussian
// Description: generate derivative gaussian
//  (-4/(sqrt(2*pi)*sigma)*(t-m)*exp^(-(t-m)^2/2sigma^2)))
//********************************************
class deri_gaussian : public source {
public:
	deri_gaussian (){};
	double get(const double time);
	double set(const double mag, const double freq);
private:
	double mean;
	double var;
};

#endif
