//********************************************
// File/Function: parse.cpp
// Description: 
//  define the parse function
//********************************************

#define BUF_SIZE 128
#include <cstdio>
#include <cstring>
#include <cmath>
#include "parse.h"

Parser::Parser(){
	hasinitial = bool;
}

//********************************************
// Function: parse
// Description: 
//  get the config file name
//  config file define:
//   define solve space size.
//   basic capacitance, inductance,
//   structure support circle, rectangle,
//   boundary structure,
//   directly generate structure
//********************************************
//void Parser::parse(string filename, 
//		int &Nx, int &Ny, double* &c, double* &l, 
//		double &deltax, double &deltay, double &deltat );
void Parser::parse(string filename, 
		double* &c, double* &l, 
		double &Rs, double &Rl, double &t){
	char buf[BUF_SIZE];
	double capacitance;
	double inductance;
	double max_f;
	int lambda_sec; //the minimum ratio of lambda to deltax
	                //recommend >= 10
	int StrucNum;
	FILE* fd;
	openfile(fd, filename);
	//fscanf("%lf %lf", &xsize, &ysize);
	fscanf("%lf %lf", &xsize);
	fscanf("%lf %lf", &capacitance, &inductance);
	fscanf("%lf %lf", &, &inductance);
	fscanf("%lf %d", &max_f, min_ratio;
	//calculate deltax size by the data above
	//initial c, l memory
	double u = 1 / sqrt(capacitance * inductance);
	double period = 1/frequency;
	double lambda = u*period;
	//set space section
	Nx = ceil((xsize/lambda)*lambda_sec);
	//Ny = ceil((ysize/lambda)*lambda_sec);
	deltax = xsize/Nx; 
	//deltay = ysize/Ny;
	c = new double[Nx];
	l = new double[Nx];
	fill(c, c+Nx, capacitance);
	fill(l, l+Nx, capacitance);
	//c = new double[Nx][Ny];
	//l = new double[Nx][Ny];
	hasinitial = true;
}
