#ifndef _LAYER_
#define _LAYER_
#include <complex>
using namespace std;
class layer {
	friend class simulation;
		int idx;
		double thickness;
		bool fitting;
	public:
		layer(int,double,int);
};
#endif //_LAYER_
