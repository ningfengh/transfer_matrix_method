#ifndef _ELLIPSOMETRY_
#define _ELLIPSOMETRY_
#include "simulation.h"
#include <cstdlib>
using namespace std;
class simulation_ellip:public simulation{
		vector<complex<double>>	psi_delta;
		vector<double> psi;
		vector<double> delta;
	public:
		void get_psi_delta(void);	
		void get_psi_delta(string);
		simulation_ellip(string,double);
};

class ellipsomtry {
	
	
};

#endif //_ELLIPSOMETRY_
