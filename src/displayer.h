#ifndef _DISPLAYER_H_H
#define _DISPLAYER_H_H

#include <string>
#include "gnuplot.h"
#include "mesh.h"

class displayer {
public:
	displayer(int _Nx, int _Ny);
	void plot(mesh* m, string name = "default");
private:
	int Nx, Ny;
	vector<double> x;
	vector<double> y;
	vector<double> z;
	Gnuplot g1;
};

#endif /* end of include guard: _DISPLAYER_H_H */
