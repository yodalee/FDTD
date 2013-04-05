#ifndef _FDTD_H_
#define _FDTD_H_

#include <string>
#include <cstdio>
#include "constant.h"
#include "source.h"

class FDTD {
public:
	FDTD (){};
	~FDTD();
	void setStruct(string setting_file);
	void setSource(source* s);
	void solve();
	void solveone();
private:
	void openfile(FILE* &fd, string filename);
	void genCircle(FILE* &fd);
	void genRect(FILE* &fd);
	void updateHz();
	void updateEx();
	void updateEy();
private:
	//one time data
	float max_frequency;
	//structure
	int Nx;
	int Ny;
	float xsize;
	float ysize;
	float deltax;
	float deltay;
	float deltat;
	source* input;
	
	int max_iteration;
	float time;
	//array type data
	double* CEx;
	double* CEy;
	double* DHz;
	//update data
	double* Ex;
	double* Ey;
	double* Hz;
	double* _Ex;
	double* _Ey;
	double* _Hz;
};

#endif
