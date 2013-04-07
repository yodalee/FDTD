#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
using namespace std;

#include "FDTD.h"

FDTD::~FDTD () {
	for (int i = 0; i < Ny+1; ++i) {
		delete []m[i];
	}
	delete[] m;
}

//need a proper way to output
void FDTD::solve(){
	for (int iterate = 0; iterate < max_iteration; ++iterate) {
		time = iterate*Dt;
		cout << time << " ";
		solveone();
		//move data
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
//  [Nx+1][Ny+1] row and column are idle for Hz, won't update
//********************************************
void FDTD::solveone(){
	for (int i = 0; i < Nx; ++i) {
		for (int j = 0; j < Ny; ++j) {
			m[i][j].Hz = m[i][j].Hz + Dt/Ds/m[i][j].eps*
			   ( m[i][j+1].Ex - m[i][j].Ex + m[i+1][j].Ey - m[i][j].Ey);
			m[i][j].Ex = m[i][j].Ex + Dt/Ds/m[i][j].eps * (m[i][j+1].Hz - m[i][j].Hz);
			m[i][j].Ey = m[i][j].Ey + Dt/Ds/m[i][j].eps * (m[i+1][j].Hz - m[i][j].Hz);
		}
	}
};

//********************************************
// Function: setStruct
// Description: 
//  parse setting file the get the parameter
// Structure limit:
//	propagation speed u, time slice Dt, space slice Ds
//	space limit: lambda > 10*Ds
//	time limit:
//	1. courant limit: u * Dt / Ds <= 1 -> Dt = 0.5*Ds/u;
//	2. nyquist theorem -> Dt = 0.5 / nyquist;
//	3. (1-2*Z/Rl|Rs) must within -1~1 -> Dt = 0.5*capacitance*Ds/min(Rs,Rl)
//********************************************
void FDTD::setStruct(string setting_file){
	int lambda_sec; //lambda/Dx default 10
	int StrucNum;
	char buf;
	//*************
	FILE* fd;
	openfile(fd, setting_file);
	fscanf(fd, "%f %f\n", &xsize, &ysize);
	fscanf(fd, "%f %d\n", &max_frequency, &lambda_sec);
	fscanf(fd, "%f\n", &time);
	fscanf(fd, "%d\n", &StrucNum);
	fscanf(fd, "source: ");
	//calculate Ds size by the data above
	//initial c, l memory
	float period = 1/max_frequency;
	float lambda = cspeed*period;
	Ds = lambda/lambda_sec;
	Nx = ceil(xsize/Ds);//set space section
	Ny = ceil(ysize/Ds);

	double nyquist = input->set(1.0, max_frequency);
	//set time section
	double limit[2];
	limit[0] = Ds/(cspeed*sqrt(2));
	limit[1] = 1 / nyquist;
	Dt = 0.5*period/ceil(period/(min(limit[0], limit[1])));
	max_iteration = ceil(time/Dt);

	time = 0;
	//initialize the memory space
	initialmesh(Nx, Ny);
	//for (int i = 0; i < StrucNum; ++i) {
	//	fscanf(fd, "%c\n", &buf);
	//	if (buf == 'c') {  //create circle
	//		genCircle(fd);
	//	} else if (buf == 'r') {  //create rectangle
	//		genRect(fd);
	//	} else {
	//		cerr << "wrong shape format" << endl;
	//		exit(EXIT_FAILURE);
	//	}
	//}
	fclose(fd);
	//need a better way to represent source
};

//********************************************
// Function: initialmesh
// Description: initial memory
//********************************************
void FDTD::initialmesh(int Nx, int Ny)
{
	m = new mesh*[Nx+1];
	for (int i = 0; i < Nx; ++i) {
		m[i] = new mesh[Ny+1];
	}
}

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
	//float cx, cy, radius, permittivity, permeability;
	//fscanf(fd, "%f %f %f\n", &cx, &cy, &radius);
	//fscanf(fd, "%f %f\n", &permittivity, &permeability);
	//float xu = cx+radius;
	//float xl = cx-radius;
	//float yu = cy+radius;
	//float yl = cy-radius;
	//if ((xu > xsize) or (yu > ysize) or (xl < 0) or (yl < 0)) {
	//	cerr << "circle shape format invalid" << endl;
	//	exit(EXIT_FAILURE);
	//}
	//cout << "create a circle center " << cx << "," << cy << " radius " << radius << endl;
	//for (int i = 0; i < Nx; ++i) {
	//	for (int j = 0; j < Ny; ++j) {
	//		float posx = (i+0.5)*Dx;
	//		float posy = j*Dy;
	//		int idx = i*Ny + j;
	//		if ((posx-cx)*(posx-cx) + (posy-cy)*(posy-cy) <= radius ) {
	//			CEx[idx] = 0;
	//		}
	//		posx = i*Dx;
	//		posy = (j+0.5)*Dy;
	//		if ((posx-cx)*(posx-cx) + (posy-cy)*(posy-cy) <=radius ) {
	//			CEy[idx] = 0;
	//		}
	//	}
	//}
}

//********************************************
// Function: genCircle
// Description: readfile to generate a rectangle area
//********************************************
void FDTD::genRect(FILE* &fd)
{
	//float px, py, w, h, permittivity, permeability;
	//fscanf(fd, "%f %f %f %f\n", &px, &py, &w, &h);
	//fscanf(fd, "%f %f\n", &permittivity, &permeability);
	//float xu = px+w;
	//float yu = py+h;
	//if ((xu > xsize) or (yu > ysize) or (px < 0) or (py < 0)) {
	//	cerr << "rect shape format invalid" << endl;
	//	exit(EXIT_FAILURE);
	//}
	//cout << "create a rectangle from " << px << "," << py << " to " << xu << "," << yu << endl;
	////scan in the rectangle point
	//for (int i = roundf(px); i < roundf(xu); ++i) {
	//	for (int j = roundf(py); j < roundf(yu); ++j) {
	//		int idx = i*Ny + j;
	//		CEx[idx] = 0;
	//		CEy[idx] = 0;
	//	}
	//}
}

