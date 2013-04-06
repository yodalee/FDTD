#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
using namespace std;

#include "FDTD.h"

FDTD::~FDTD () {
	delete []CEx;
	delete []CEy;
	delete []DHz;
	delete []Ex;
	delete []Hz;
	delete []_Ex;
	delete []_Hz;
	delete []Ey;
	delete []_Ey;
}

//need a proper way to output
void FDTD::solve(){
	for (int iterate = 0; iterate < max_iteration; ++iterate) {
		time = iterate*deltat;
		cout << time << " ";
		solveone();
		//move data
		swap(Ex,_Ex);
		swap(Ey,_Ey);
		swap(Hz,_Hz);
		cout << endl;
	}
};
//********************************************
// Function: solveone
// Description: update Ex, Ey, Hz
//  ----Ex2-----
//  |          |
//  |          |
// Ey1   Hz   Ey2
//  |          |
//  |          |
//  ----Ex1-----
//  
// Ex = new double[Nx+1][Ny+1](); //Nx index is idle
// Ey = new double[Nx+1][Ny+1](); //Ny index is idle
// Hz = new double[Nx+1][Ny+1]();	   //Nx and Ny index is idle
//********************************************
void FDTD::solveone(){
	updateHz();
	updateEx();
	updateEy();
};

// update from X to _X
void FDTD::updateHz()
{
	for (int i = 0; i < Nx; ++i) {
		for (int j = 0; j < Ny; ++j) {
			int idx = i*Ny + j;
			_Hz[idx] = Hz[idx] + DHz[idx]*
				(Ex[idx+1] - Ex[idx] + Ey[idx+Ny] - Ey[idx]);
		}
	}
}
void FDTD::updateEx()
{
	for (int i = 0; i < Nx; ++i) {
		for (int j = 1; j < Ny; ++j) {  //only update the center part
			int idx = i*Ny + j;
			_Ex[idx] = Ex[idx] + CEx[idx]* (Hz[idx] - Hz[idx-Ny]);
		}
		_Ex[i*Ny] = 0; //PEC boundary condition
		_Ex[i*Ny+Ny] = 0;
	}
}
void FDTD::updateEy()
{
	for (int j = 0; j < Ny; ++j) {
		for (int i = 1; i < Nx; ++i) {  //only update the center part
			int idx = i*Ny + j;
			_Ey[idx] = Ey[idx] + CEy[idx]* (Hz[idx] - Hz[idx-Ny]);
		}
		_Ex[j] = 0; //PEC boundary condition
		_Ex[(Nx-1)*Ny+j] = 0;
	}
}

//********************************************
// Function: setStruct
// Description: 
//  parse setting file the get the parameter
// Structure limit:
//	propagation speed u, time slice deltat, space slice deltax	
//	space limit: lambda > 10*deltax
//	time limit:
//	1. courant limit: u * deltat / deltax <= 1 -> deltat = 0.5*deltax/u;
//	2. nyquist theorem -> deltat = 0.5 / nyquist;
//	3. (1-2*Z/Rl|Rs) must within -1~1 -> deltat = 0.5*capacitance*deltax/min(Rs,Rl)
//********************************************
void FDTD::setStruct(string setting_file){
	int lambda_sec; //lambda/deltax default 10
	int StrucNum;
	char buf;
	//*************
	FILE* fd;
	openfile(fd, setting_file);
	fscanf(fd, "%f %f\n", &xsize, &ysize);
	fscanf(fd, "%f %d\n", &max_frequency, &lambda_sec);
	fscanf(fd, "%f\n", &time);
	fscanf(fd, "%d\n", &StrucNum);
	//calculate deltax size by the data above
	//initial c, l memory
	float period = 1/max_frequency;
	float lambda = cspeed*period;
	Nx = ceil((xsize/lambda)*lambda_sec);//set space section
	Ny = ceil((ysize/lambda)*lambda_sec);
	deltax = xsize/Nx; 
	deltay = ysize/Ny;

	double nyquist = input->set(1.0, max_frequency);
	//set time section
	double limit[2];
	limit[0] = min(deltax,deltay)/(cspeed*sqrt(2));
	limit[1] = 1 / nyquist;
	deltat = 0.5*period/ceil(period/(min(limit[0], limit[1])));
	max_iteration = ceil(time/deltat);

	time = 0;
	//initialize the memory space
	CEx = new double[Nx*Ny];
	CEy = new double[Nx*Ny];
	DHz = new double[Nx*Ny];

	fill(CEx, CEx + Nx*Ny, deltat/eps );
	fill(CEy, CEy + Nx*Ny, deltat/eps );
	fill(DHz, DHz + Nx*Ny, deltat/mu );
	for (int i = 0; i < StrucNum; ++i) {
		fscanf(fd, "%c\n", &buf);
		if (buf == 'c') {  //create circle
			genCircle(fd);
		} else if (buf == 'r') {  //create rectangle
			genRect(fd);
		} else {
			cerr << "wrong shape format" << endl;
			exit(EXIT_FAILURE);
		}
	}
	fclose(fd);
	//need a better way to represent source
	Hz  = new double[Nx*Ny]();
	_Hz = new double[Nx*Ny]();
	Ex  = new double[Nx*(Ny+1)]();
	_Ex = new double[Nx*(Ny+1)]();
	Ey  = new double[(Nx+1)*Ny]();
	_Ey = new double[(Nx+1)*Ny]();
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

//********************************************
// Function: genCircle
// Description: readfile to generate a circle area
//********************************************
void FDTD::genCircle(FILE* &fd)
{
	float cx, cy, radius, permittivity, permeability;
	fscanf(fd, "%f %f %f\n", &cx, &cy, &radius);
	fscanf(fd, "%f %f\n", &permittivity, &permeability);
	float xu = cx+radius;
	float xl = cx-radius;
	float yu = cy+radius;
	float yl = cy-radius;
	if ((xu > xsize) or (yu > ysize) or (xl < 0) or (yl < 0)) {
		cerr << "circle shape format invalid" << endl;
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < Nx; ++i) {
		for (int j = 0; j < Ny; ++j) {
			float posx = (i+0.5)*deltax;
			float posy = j*deltay;
			int idx = i*Ny + j;
			if ((posx-cx)*(posx-cx) + (posy-cy)*(posy-cy) <= radius ) {
				CEx[idx] = 0;
			}
			posx = i*deltax;
			posy = (j+0.5)*deltay;
			if ((posx-cx)*(posx-cx) + (posy-cy)*(posy-cy) <=radius ) {
				CEy[idx] = 0;
			}
		}
	}
}

//********************************************
// Function: genCircle
// Description: readfile to generate a rectangle area
//********************************************
void FDTD::genRect(FILE* &fd)
{
	float px, py, w, h, permittivity, permeability;
	fscanf(fd, "%f %f %f %f\n", &px, &py, &w, &h);
	fscanf(fd, "%f %f\n", &permittivity, &permeability);
	float xu = px+w;
	float yu = py+h;
	if ((xu > xsize) or (yu > ysize) or (px < 0) or (py < 0)) {
		cerr << "rect shape format invalid" << endl;
		exit(EXIT_FAILURE);
	}
	//scan in the rectangle point
	for (int i = roundf(px); i < roundf(xu); ++i) {
		for (int j = roundf(py); j < roundf(yu); ++j) {
			int idx = i*Ny + j;
			CEx[idx] = 0;
			CEy[idx] = 0;
		}
	}
}

