#include "mesh.h"

double mesh::Ds = 0;
double mesh::Dt = 0;
//void mesh::setstatic(double, double);

mesh::mesh(double m, double e){
	Ex = 0;
	Ey = 0;
	Hz = 0;
	DH = Dt/(Ds*m);
	CE = Dt/(Ds*e);
	//mu = m;
	//eps = e;
}

void mesh::setMaterial(double m, double e, double sigma){
	DH = Dt/(Ds*m);
	CE = Dt/(Ds*e);
	//mu = m;
	//eps = e;
}

void mesh::setstatic(double _Ds, double _Dt){
	mesh::Ds = _Ds;
	mesh::Dt = _Dt;
}
