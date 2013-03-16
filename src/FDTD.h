#include "constant.h"

class FDTD {
public:
	FDTD (double _c, double _l, double _f, int _sec);
	~FDTD(){
		delete []_V;
		delete []_I;
		delete []V;
		delete []I;
	}
	void initialStruct();
	void solve();
	void solveone(const double* V1, const double* I1, double* V2, double* I2);
	void output();
private:
	double capacitance;
	double inductance;
	double frequency;
	double delta_x;
	double delta_t;
	double Rs;
	double Rl;
	
	int gridi_bound;
	int iteration;
	int sec;
	double time;
	double* V;
	double* I;
	double* _V;
	double* _I;
};
