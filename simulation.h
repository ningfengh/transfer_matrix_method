#ifndef _SIMULATION_
#define _SIMULATION_
#define M_PI           3.14159265358979323846
#include <vector>
#include <string>
#include <complex>
#include <cmath>
#include "layer.h"
#include "material.h"
using namespace std;
class simulation{

		double wav_begin;
		double wav_end;
		double npoint;
		double aoi;
		vector<double> 	wav_vector;
		vector<double> 	ref_vector;
		vector<double> 	trans_vector;
		vector<material*> material_data;
		vector<layer*>	layer_data;
	public:
		simulation(string); // filename
		void get_ref_trans(string);
	
};

#endif //_SIMULATION_