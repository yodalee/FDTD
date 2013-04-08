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
		void setMaterial(double m, double e, double sigma=0);
	public:
		static void setstatic(double Ds, double Dt);
		static double Ds;
		static double Dt;
		double Ex, Ey, Hz;
		double mu;
		double eps;
};

#endif
