#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
using namespace std;

#include "FDTD.h"

FDTD::FDTD (double _c, double _l, double _f, double x, int N, double time, int t_sec, double rs, double rl, string source){
	capacitance = _c;
	inductance = _l;
	frequency = _f;
	Rs = rs;
	Rl = rl;
	initialStruct(x, N, time, t_sec, source);
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

//need a proper way to output
void FDTD::solve(){
	for (int iterate = 0; iterate < max_iteration; ++iterate) {
		time = iterate*delta_t;
		cout << time << " ";
		if (iterate%2 == 0) {
			solveone(V,I,_V,_I); //calculate solution by V,I to _V,_I
			for (int idx = 0; idx < gridi_bound+1; ++idx) { cout << _V[idx] << " "; }
		} else {
			solveone(_V,_I,V,I); //calculate solution by V,I to _V,_I
			for (int idx = 0; idx < gridi_bound+1; ++idx) { cout << V[idx] << " "; }
		}
		cout << endl;
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
// Description: 
//  initial structure parameter and check within limit
//  initial memory
//
// Structure limit:
//	propagation speed u, time slice delta_t, space slice delta_x	
//	1. wave length section: lambda > 10*delta_x
//	2. courant limit: u * delta_t / delta_x <= 1
//	detemine the delta_t
//********************************************
void FDTD::initialStruct(double x, int xsec, double t, int tsec, string type){
	input = generate_source(type);
	double nyquist = input->set(1.0, frequency);
	//set the source
	double u = 1 / sqrt(capacitance * inductance);
	double lambda = u/frequency;
	delta_x = x/xsec;
	delta_t = t/tsec;
	max_iteration = tsec;
	//check limit
	if (lambda < 10*delta_x) { cerr << "x section too large\n"; }
	if (delta_t >= 0.5*delta_x/u) { cerr << "doesn't fit courant limit\n";}
	if (delta_t * nyquist > 1) { cerr << "doesn't fit nyquist limit\n";}
	time = 0;
	//using the twice of courant limit
	//int gridi = 256;
	gridi_bound = xsec;
	//initialize the memory space
	V = new double[gridi_bound+1]();
	I = new double[gridi_bound]();
	_V = new double[gridi_bound+1]();
	_I = new double[gridi_bound]();
};
