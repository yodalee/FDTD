#include "mesh.h"

mesh::mesh(double m, double e){
	Ex = 0;
	Ey = 0;
	Hz = 0;
	mu = m;
	eps = e;
}

void mesh::setMaterial(double m, double e, double sigma){
	mu = m;
	eps = e;
}
