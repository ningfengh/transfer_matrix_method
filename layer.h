#ifndef _LAYER_
#define _LAYER_
#include <complex>
using namespace std;
class layer {
	friend class simulation;
		int idx;
		double thickness;
	public:
		layer(int,double);
};
#endif //_LAYER_