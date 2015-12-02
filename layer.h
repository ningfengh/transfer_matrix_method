#ifndef _LAYER_
#define _LAYER_
#include <complex>
#include <iostream>
using namespace std;
class layer {
	friend class simulation;
		int idx;
		double thickness;
		bool fitting;
	public:
		layer(int,double,int);
		bool is_variable(void);
		double get_thickness(void);
		void set_thickness(double);
};
#endif //_LAYER_
