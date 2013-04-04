#ifndef _FDTD_H_
#define _FDTD_H_

#include <string>
#include <cstdio>
#include "constant.h"
#include "source.h"

class FDTD {
public:
	FDTD ();
	~FDTD(){
		delete []_V;
		delete []_I;
		delete []V;
		delete []I;
		delete []Z;
		delete []G;
	}
	void setStruct(string setting_file);
	void setSource(source* s);
	void solve();
	void solveone(const double* V1, const double* I1, double* V2, double* I2);
private:
	void openfile(FILE* &fd, string filename);
private:
	//one time data
	double max_frequency;
	//structure
	int Nx;
	double deltax;
	double deltat;
	double Rs;
	double Rl;
	source* input;
	
	int max_iteration;
	float time;
	//array type data
	double* Z;
	double* G;
	//update data
	double* V;
	double* I;
	double* _V;
	double* _I;
};

#endif
