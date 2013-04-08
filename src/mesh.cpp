#include "mesh.h"

double mesh::Ds = 0;
double mesh::Dt = 0;

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

void setstatic(double _Ds, double _Dt){
	mesh::Ds = _Ds;
	mesh::Dt = _Dt;
}
