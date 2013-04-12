//********************************************
// File: mesh.h
// Description: provide the mesh struct
//  with default value assign
//  record Ex, Ey, Hz, mu, epsilon of a mesh
//********************************************

#ifndef _MESH_H_
#define _MESH_H_

#include "constant.h"

typedef struct mesh mesh;
struct mesh {
	public:
		mesh(double m = mu0, double e = eps0);
		void setMaterial(double m, double e,
			double sigx=0, double sigy=0,double sigx_=0, double sigy_=0);
		void setMaterial(string type);
	public:
		static void setstatic(double Ds, double Dt);
		static double Ds;
		static double Dt;
		double Ex, Ey, Hzx, Hzy;
		double CEx1, CEx2;
		double CEy1, CEy2;
		double DHx1, DHx2;
		double DHy1, DHy2;
};

#endif
