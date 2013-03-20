#include <string>
#include "constant.h"
#include "source.h"

class FDTD {
public:
	FDTD (double _c, double _l, double _f, double x, int N, double time, int t_sec, double rs, double rl, string source);
	~FDTD(){
		delete []_V;
		delete []_I;
		delete []V;
		delete []I;
	}
	source* generate_source(string type){ return createsource(type);}
	source* createsource(string type);
	void initialStruct(double x, int xsec, double time, int tsec, string type);
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
	source* input;
	
	int gridi_bound;
	int max_iteration;
	double time;
	double* V;
	double* I;
	double* _V;
	double* _I;
};
