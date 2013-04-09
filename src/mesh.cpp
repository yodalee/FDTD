#include "mesh.h"

double mesh::Ds = 0;
double mesh::Dt = 0;

mesh::mesh(double m, double e){
	Ex = 0;
	Ey = 0;
	Hzx = 0;
	Hzy = 0;
	setMaterial(m,e);
}

void mesh::setMaterial(
		double m, double e, double sigx, double sigy){
	double term=0;
	double sigx_ = sigx * imp0 * imp0;
	double sigy_ = sigy * imp0 * imp0;
	term=0.5*(sigy*Dt)/(e);
	CEx1 = (1-term)/(1+term);
	CEx2 = (Dt/(e*Ds))/(1+term);
	term=0.5*(sigx*Dt)/(e);
	CEy1 = (1-term)/(1+term);
	CEy2 = (Dt/(e*Ds))/(1+term);
	term=0.5*(sigx_*Dt)/(m);
	DHx1 = (1-term)/(1+term);
	DHx2 = (Dt/(m*Ds))/(1+term);
	term=0.5*(sigy_*Dt)/(m);
	DHy1 = (1-term)/(1+term);
	DHy2 = (Dt/(m*Ds))/(1+term);
}

void mesh::setstatic(double _Ds, double _Dt){
	mesh::Ds = _Ds;
	mesh::Dt = _Dt;
}
