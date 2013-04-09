#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <cstdio>
#include <vector>
using namespace std;

#include "FDTD.h"
#include "gnuplot_i.hpp"

FDTD::~FDTD () {
	for (int i = 0; i < Ny+1; ++i) {
		delete []m[i];
	}
	delete[] m;
}

void FDTD::solve(){
	Gnuplot g1("lines");
	g1.reset_plot();
	g1.set_yrange(-10,10);
	vector<double> x((Nx+1)), y((Nx+1)*(Ny+1));
	vector<double> z((Nx+1));
	for (int i = 0; i < Nx+1; ++i) {
		for (int j = 0; j < Ny+1; ++j) {
			 x[i*(1)] = i;
			 y[i*(Ny+1)+j] = j;
		}
	}
	//int idy = 0.5*Ny;
	for (int iterate = 0; iterate < iteration; ++iterate) {
		time += Dt;
		solveone();
		if ((iterate&15) == 0) {
			usleep(1000);
			g1.reset_plot();
			for (int i = 0; i < Nx+1; ++i) {
				//for (int j = 0; j < Ny+h; ++j) {
					z[i*(1)] = m[i][Ny/2].Ey;
				//}
			}
			//g1.set_style("image");
			g1.plot_xy(x,z, "plot");
		}
	} 
	cerr << "press key:";
	fgetc(stdin);

};

//********************************************
// Function: solveone
// Description: update Ex, Ey, Hz
// the mesh[1][1] of Ex, Ey, Hz define as below
//  ------------
//  |          |
//  |          |
// Ey1  Hz1    |
// CE   DH     |
//  |          |
//  ----Ex1-----
//		CE
//  mesh[Nx+1][Ny+1] Ex Ey idle for Hz, won't update
//********************************************
void FDTD::solveone(){
	//update
	for (int i = 0; i < Nx; ++i) {
		for (int j = 0; j < Ny; ++j) {
			m[i][j].Hzx = m[i][j].DHx1*m[i][j].Hzx - m[i][j].DHx2 * (m[i+1][j].Ey - m[i][j].Ey);
			m[i][j].Hzy = m[i][j].DHy1*m[i][j].Hzy + m[i][j].DHy2 * (m[i][j+1].Ex - m[i][j].Ex);
		}
	}
	for (int i = 1; i < Nx+1; ++i) {
		for (int j = 1; j < Ny+1; ++j) {
			m[i][j].Ex = m[i][j].CEx1*m[i][j].Ex + m[i][j].CEx2 * (m[i][j].Hzx + m[i][j].Hzy - m[i][j-1].Hzx - m[i][j-1].Hzy);
			m[i][j].Ey = m[i][j].CEy1*m[i][j].Ey - m[i][j].CEy2 * (m[i][j].Hzx + m[i][j].Hzy - m[i-1][j].Hzx - m[i-1][j].Hzy);
		}
	}
	//impulse
	if (time < 1e-9) {
		//add source
		int idx = 0.5*Nx;
		int idy = 0.5*Ny;
		double Esource = input->get(time);
		double Hsource = input->get(time-0.5*Dt)/imp0;
		for (int j = 1; j < Ny; ++j) {
			m[idx][j].Hzy	+= m[idx][j].DHx2*Esource;
			m[idx+1][j].Ey	+= m[idx+1][j].CEy2*Hsource;
		}
	}
	//pec
	for (int i = 1; i < Nx+1; ++i) { m[i][Ny].Ex = 0; }
	for (int j = 0; j < Ny+1; ++j) { m[Nx][j].Ey = 0; }
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
	float max_frequency;
	//*************
	FILE* fd;
	openfile(fd, setting_file);
	fscanf(fd, "%d %d\n", &Nx, &Ny);
	fscanf(fd, "%f\n", &Ds);
	fscanf(fd, "%f %d\n", &max_frequency, &lambda_sec);
	fscanf(fd, "%d\n", &iteration);
	fscanf(fd, "%d\n", &StrucNum);
	fscanf(fd, "source: ");

	double nyquist = input->set(7, max_frequency);
	Dt = 0.9*Ds/(cspeed*sqrt(2));
	mesh::setstatic(Ds, Dt);
	time = 0;
	//initialize the memory space
	initialmesh(Nx, Ny);
	//initial PML structure
	int PMLm = 2;
	int layer = 10;
	double inv = 1.0/layer;
	double sigma_max = 0.8*(PMLm+1)/(imp0*Ds);
	//top bottom
	for (int i = 0; i < Nx+1; ++i) {
		for (int j = 0; j < layer; ++j) {
			double sigy = pow((layer-j)*inv,PMLm)*sigma_max;
			double sigy1_ = imp0*imp0*pow((layer-j-0.5)*inv,PMLm)*sigma_max;
			double sigy2_ = imp0*imp0*pow((layer-j+0.5)*inv,PMLm)*sigma_max;
			m[i][j].   setMaterial(mu0,eps0,0,0,sigy,sigy1_);
			m[i][Ny-1-j].setMaterial(mu0,eps0,0,0,sigy,sigy2_);
		}
	}
	//left right
	for (int i = 0; i < layer; ++i) {
		for (int j = 0; j < Ny+1; ++j) {
			double sigx = pow((layer-i)*inv,PMLm)*sigma_max;
			double sigx1_ = imp0*imp0*pow((layer-i-0.5)*inv,PMLm)*sigma_max;
			double sigx2_ = imp0*imp0*pow((layer-i+0.5)*inv,PMLm)*sigma_max;
			m[i][j].setMaterial(mu0,eps0,sigx,sigx1_);
			m[Nx-1-i][j].setMaterial(mu0,eps0,sigx,sigx2_);
		}
	}
	//corner
	for (int i = 0; i < layer; ++i) {
		for (int j = 0; j < layer; ++j) {
			double sigx = pow((layer-i)*inv,PMLm)*sigma_max;
			double sigx1_ = imp0*imp0*pow((layer-i-0.5)*inv,PMLm)*sigma_max;
			double sigx2_ = imp0*imp0*pow((layer-i+0.5)*inv,PMLm)*sigma_max;
			double sigy = pow((layer-j)*inv,PMLm)*sigma_max;
			double sigy1_ = imp0*imp0*pow((layer-j-0.5)*inv,PMLm)*sigma_max;
			double sigy2_ = imp0*imp0*pow((layer-j+0.5)*inv,PMLm)*sigma_max;
			m[i][j].setMaterial(mu0,eps0,sigx,sigx1_,sigy,sigy1_);
			m[Nx-1-i][j].setMaterial(mu0,eps0,sigx,sigx2_,sigy,sigy1_);
			m[i][Ny-1-j].setMaterial(mu0,eps0,sigx,sigx1_,sigy,sigy2_);
			m[Nx-1-i][Ny-1-j].setMaterial(mu0,eps0,sigx,sigx2_,sigy,sigy2_);
		}
	}
	//initial special structure
	for (int i = 0; i < StrucNum; ++i) {
		fscanf(fd, "%c\n", &buf);
		if (buf == 'c') {  //create circle
			genCircle(fd);
	//	} else if (buf == 'r') {  //create rectangle
	//		genRect(fd);
	//	} else {
	//		cerr << "wrong shape format" << endl;
	//		exit(EXIT_FAILURE);
		}
	}
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
	for (int i = 0; i < Nx+1; ++i) {
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
	int cx, cy;
	float radius, eps, mu;
	fscanf(fd, "%d %d %f\n", &cx, &cy, &radius);
	fscanf(fd, "%f %f\n", &eps, &mu);
	int r = floor(radius/Ds);
	int xu = cx+r;
	int xl = cx-r;
	int yu = cy+r;
	int yl = cy-r;
	if ((xu > Nx) or (yu > Ny) or (xl < 0) or (yl < 0)) {
		cerr << "circle shape format invalid" << endl;
		exit(EXIT_FAILURE);
	}
	cout << "create a circle center " << cx << "," << cy << " radius " << radius << endl;
	for (int i = 0; i < Nx+1; ++i) {
		for (int j = 0; j < Ny+1; ++j) {
			if ((cx-i)*(cx-i) + (cy-j)*(cy-j) <= radius*radius) {
				m[i][j].setMaterial(mu, eps);
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

