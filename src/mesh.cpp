#include <string>
using std::string;
#include "mesh.h"

double mesh::Ds = 0;
double mesh::Dt = 0;
//void mesh::setstatic(double, double);

mesh::mesh(double m, double e){
	Ex = 0;
	Ey = 0;
	Hzx = 0;
	Hzy = 0;
	setMaterial(m,e);
}

void mesh::setMaterial(double m, double e,
		double sigx, double sigx_,double sigy, double sigy_){
	double term = 0;
	term=0.5*(sigy*Dt)/(e);
	CEx1 = (1.0-term)/(1.0+term);
	CEx2 = (Dt/(e*Ds))/(1.0+term);
	term=0.5*(sigx*Dt)/(e);
	CEy1 = (1.0-term)/(1.0+term);
	CEy2 = (Dt/(e*Ds))/(1.0+term);
	term=0.5*(sigx_*Dt)/(m);
	DHx1 = (1.0-term)/(1.0+term);
	DHx2 = (Dt/(m*Ds))/(1.0+term);
	term=0.5*(sigy_*Dt)/(m);
	DHy1 = (1.0-term)/(1.0+0.5*(sigx_*Dt)/(m));
	DHy2 = (Dt/(m*Ds))/(1.0+term);
}

void mesh::setMaterial(string type){
	if (type == "pec") {
		CEx1 = 1;
		CEx2 = 0;
		CEy1 = 1;
		CEy2 = 0;
	}
}

void mesh::setstatic(double _Ds, double _Dt){
	mesh::Ds = _Ds;
	mesh::Dt = _Dt;
}
