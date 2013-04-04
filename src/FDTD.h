#include <string>
#include "constant.h"
#include "source.h"

class FDTD {
public:
	FDTD (string setting_file);
	~FDTD(){
		delete []_V;
		delete []_I;
		delete []V;
		delete []I;
	}
	void setStruct(string setting_file);
	void setSource(source* s);
	void setSim(double max_frequency);
	void solve();
	void solveone(const double* V1, const double* I1, double* V2, double* I2);
private:
	double delta_x;
	double delta_t;
	double Rs;
	double Rl;
	source* input;
	
	int Nx;
	int max_iteration;
	double time;
	//array type data
	double* capacitance;
	double* inductance;
	//update data
	double* V;
	double* I;
	double* _V;
	double* _I;
};
