#include <iostream>
using namespace std;

#include "displayer.h"

displayer::displayer(int _Nx, int _Ny) : g1("lines"){
	Nx = _Nx;
	Ny = _Ny;
	x.resize(Nx*Ny);
	y.resize(Nx*Ny);
	z.resize(Nx*Ny);
	for (int i = 0; i < Nx; ++i) {
		for (int j = 0; j < Ny; ++j) {
			 x[i*Ny+j] = i;
			 y[i*Ny+j] = j;
		}
	}
	g1.set_cbrange(-10,10);
}

void displayer::plot(mesh* m, string name){
	g1.savetops(name, Nx+1, Ny+1);
	g1.reset_plot();
	for (int i = 0; i < Nx; ++i) {
		for (int j = 0; j < Ny; ++j) {
			z[i*Ny+j] = m[i*Ny+j].Ey;
		}
	}
	g1.set_style("image");
	g1.set_view("map");
	g1.plot_xyz(x,y,z, "plot");
}
