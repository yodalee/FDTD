#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
using namespace std;

#include "FDTD.h"

FDTD::FDTD(double _c, double _l, double _f, int _sec, string source){
	capacitance = _c;
	inductance = _l;
	frequency = _f;
	Rs = 1;
	Rl = 2;
	max_iteration = 500;
	initialStruct(_sec, source);
}

source* FDTD::createsource(string type){
	if (type == "gaussian") {
		return (new gaussian);
	} else if(type == "single_frequency") {
		return (new single_frequency);
	} else {
		std::cerr << "invalid source option!\n";
		exit(EXIT_FAILURE);
	}
}

void FDTD::solve(){
	for (int iterate = 0; iterate < max_iteration; ++iterate) {
		solveone(V,I,_V,_I); //calculate solution by V,I to _V,_I
		//copy solution data from _V,_I to V,I
		for (int copy_idx = 0; copy_idx != gridi_bound; ++copy_idx) {
			V[copy_idx] = _V[copy_idx];
			I[copy_idx] = _I[copy_idx];
		}
		V[gridi_bound] = _V[gridi_bound];
		//need a proper way to 
		for (int idx = 0; idx < gridi_bound+1; ++idx) {
			cout << V[idx] << " ";
		}
		cout << endl;
		time += delta_t;
	}
};
//********************************************
// Function: solveone
// Description: solve one will initial V2,I2
//	calculate the V2,I2 by old V1,I1 with update equation
//	then delete V1, I1
//********************************************
void FDTD::solveone(const double* V1, const double* I1, double* V2, double* I2){
	//left bound
	double Z = delta_t/(capacitance*delta_x);
	double G = delta_t/(inductance*delta_x);
	//need a better way to represent source
	V2[0] = (1-2*Z/Rs)*V1[0] - 2*Z*I1[0] + (2*Z/Rs)*(input->get(time));
	//right bound
	V2[gridi_bound] = (1-2*Z/Rl)*V1[gridi_bound] + 2*Z*I1[gridi_bound-1];
	//center zone
	for (int idx = 1; idx < gridi_bound; ++idx) {
		V2[idx] = V1[idx] - Z*(I1[idx] - I1[idx-1]);
	}
	for (int idx = 0; idx < gridi_bound; ++idx) {
		I2[idx] = I1[idx] - G*(V2[idx+1]-V2[idx]);
	}
};

//********************************************
// Function: initialStruct
// Description: check structure limit and initial memory
//
// Structure limit:
//	propagation speed u, time slice delta_t, space slice delta_x	
//	u * delta_t / delta_x <= 1
//	detemine the delta_t
//********************************************
void FDTD::initialStruct(int sec, string type){
	input = generate_source(type);
	double nyquist = input->set(1.0, frequency);
	//set the source
	double u = 1 / sqrt(capacitance * inductance);
	double lambda = u/frequency;
	delta_x = lambda/sec; //set space section size 0.01 m
	delta_t = min(0.5*delta_x/u, 1/nyquist);
	time = 0;
	//using the twice of courant limit
	//int gridi = 256;
	gridi_bound = 16;
	//initialize the memory space
	V = new double[gridi_bound+1]();
	I = new double[gridi_bound]();
	_V = new double[gridi_bound+1]();
	_I = new double[gridi_bound]();
};
