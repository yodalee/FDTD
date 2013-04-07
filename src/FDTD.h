#ifndef _FDTD_H_
#define _FDTD_H_

#include <string>
#include <cstdio>
#include "constant.h"
#include "mesh.h"
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
	void initialmesh(int Nx, int Ny);
private:
	//structure
	int Nx;
	int Ny;
	float Ds;
	float Dt;
	source* input;
	
	int iteration;
	float time;
	
	mesh** m;
};

#endif
