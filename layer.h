#ifndef _LAYER_
#define _LAYER_
#include <complex>
using namespace std;
class layer {
		double n;
		double k;
		double thickness;
		complex<double> tmm_s[2][2];
		complex<double> tmm_p[2][2];
	public:
		layer(int,double);
};
#endif //_LAYER_