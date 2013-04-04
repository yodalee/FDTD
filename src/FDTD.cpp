#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
using namespace std;

#include "FDTD.h"

FDTD::FDTD (){ }

//need a proper way to output
void FDTD::solve(){
	for (int iterate = 0; iterate < max_iteration; ++iterate) {
		time = iterate*deltat;
		cout << time << " ";
		solveone(V,I,_V,_I);
		for (int idx = 0; idx < Nx+1; ++idx) { cout << _V[idx] << " "; }
		swap(V,_V);
		swap(I,_I);
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
	V2[0] = (1-2*Z[0]/Rs)*V1[0] - 2*Z[0]*I1[0] + (2*Z[0]/Rs)*(input->get(time));
	//right bound
	V2[Nx] = (1-2*Z[Nx]/Rl)*V1[Nx] + 2*Z[Nx]*I1[Nx-1];
	//center zone
	for (int idx = 1; idx < Nx; ++idx) {
		V2[idx] = V1[idx] - Z[idx]*(I1[idx] - I1[idx-1]);
	}
	for (int idx = 0; idx < Nx; ++idx) {
		I2[idx] = I1[idx] - G[idx]*(V2[idx+1]-V2[idx]);
	}
};

//********************************************
// Function: setStruct
// Description: 
//  parse setting file the get the parameter
// Structure limit:
//	propagation speed u, time slice delta_t, space slice delta_x	
//	1. wave length section: lambda > 10*delta_x
//	2. courant limit: u * delta_t / delta_x <= 1 -> delta_t = 0.5*delta_x/u;
//	3. nyquist theorem -> delta_t = 0.5 / nyquist;
//	4. (1-2*Z/Rl|Rs) must within -1~1 -> delta_t = 0.5*capacitance*delta_x/min(Rs,Rl)
//********************************************
void FDTD::setStruct(string setting_file){
	double xsize;
	double capacitance;
	double inductance;
	int lambda_sec; //lambda/deltax default 10
	//int StrucNum;
	//*************
	FILE* fd;
	openfile(fd, setting_file);
	fscanf(fd, "%lf", &xsize);
	fscanf(fd, "%lf %lf", &capacitance, &inductance);
	fscanf(fd, "%lf %lf", &Rs, &Rl);
	fscanf(fd, "%lf %d", &max_frequency, &lambda_sec);
	fscanf(fd, "%f", &time);
	//calculate deltax size by the data above
	//initial c, l memory
	double u = 1 / sqrt(capacitance * inductance);
	double period = 1/max_frequency;
	double lambda = u*period;
	//set space section
	Nx = ceil((xsize/lambda)*lambda_sec);
	//Ny = ceil((ysize/lambda)*lambda_sec);
	deltax = xsize/Nx; 
	//deltay = ysize/Ny;

	double nyquist = input->set(1.0, max_frequency);
	//set time section
	double limit[3];
	limit[0] = deltax/u;
	limit[1] = 1 / nyquist;
	limit[2] = capacitance*deltax*min(Rs,Rl);
	deltat = 0.5*period/ceil(period/(*min_element(limit, limit+3)));
	max_iteration = ceil(time/deltat);

	//initialize the memory space
	time = 0;
	Z = new double[Nx+1]();
	G = new double[Nx]();
	fill(Z, Z+Nx+1, deltat/(capacitance*deltax));
	fill(G, G+Nx, deltat/(inductance*deltax));
	//need a better way to represent source
	V = new double[Nx+1]();
	I = new double[Nx]();
	_V = new double[Nx+1]();
	_I = new double[Nx]();
};

//********************************************
// Function: setSource
// Description: 
//  replace the source of the FDTD
//********************************************
void FDTD::setSource(source* s){ input = s; }

//********************************************
// Function: openfile
// Description: prepare FILE* fd
//********************************************
void FDTD::openfile(FILE* &fd, string filename) {
	fd = fopen(filename.c_str(), "r");
	if (fd == NULL){
		cerr << "Cannot open file: " << filename << endl;
		exit(EXIT_FAILURE);
	}
}
