#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
using namespace std;

#include "FDTD.h"

FDTD::FDTD (string setting_file){
	setStruct(setting_file);
}

//need a proper way to output
void FDTD::solve(){
	for (int iterate = 0; iterate < max_iteration; ++iterate) {
		time = iterate*delta_t;
		cout << time << " ";
		if (iterate%2 == 0) {
			solveone(V,I,_V,_I); //calculate solution by V,I to _V,_I
			for (int idx = 0; idx < Nx+1; ++idx) { cout << _V[idx] << " "; }
		} else {
			solveone(_V,_I,V,I); //calculate solution by V,I to _V,_I
			for (int idx = 0; idx < Nx+1; ++idx) { cout << V[idx] << " "; }
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
// Function: setStruct
// Description: 
//  parse setting file the get the parameter
//********************************************

//********************************************
// Function: setSim
// Description: get the max interesting frequency
//  initial memory within limit
//
// Structure limit:
//	propagation speed u, time slice delta_t, space slice delta_x	
//	1. wave length section: lambda > 10*delta_x
//	2. courant limit: u * delta_t / delta_x <= 1 -> delta_t = 0.5*delta_x/u;
//	3. nyquist theorem -> delta_t = 0.5 / nyquist;
//	4. (1-2*Z/Rl|Rs) must within -1~1 -> delta_t = 0.5*capacitance*delta_x/min(Rs,Rl)
//********************************************
void FDTD::setSim(double max_frequency){
}

N, time
void FDTD::setStruct(string setting_file){
	//initial variable
	int Nx;
	Parser p;
	p.parse(setting_file,
			capacitance, inductance,
			Rs, Rl, t)
	double nyquist = input->set(1.0, frequency);
	//set time section
	double limit[3];
	limit[0] = deltax/u;
	limit[1] = 1 / nyquist;
	limit[2] = capacitance*deltax*min(Rs,Rl);
	delta_t = 0.5*period/ceil(period/(*min_element(limit, limit+3)));
	max_iteration = ceil(t/deltat);

	//initialize the memory space
	time = 0;
	V = new double[Nx+1]();
	I = new double[Nx]();
	_V = new double[Nx+1]();
	_I = new double[Nx]();
	
	//cout << t << "\n";
	//cout << deltat << "\n";
	//cout << deltax * Nx << "\n";
	//cout << deltax << "\n";
	//cout << type << "\n";
	//cout << endl;
};

//********************************************
// Function: setSource
// Description: 
//  replace the source of the FDTD
//********************************************
void setSource(source* s){
	input = s;
}
