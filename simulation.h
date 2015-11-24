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
		vector<complex<double>>	ref_vector_s;
		vector<complex<double>>	trans_vector_s;
		vector<complex<double>>	ref_vector_p;
		vector<complex<double>>	trans_vector_p;		
		vector<material*> material_data;
		vector<layer*>	layer_data;
	public:
		simulation(string); // filename
		void get_ref_trans(string,char);
	
};

#endif //_SIMULATION_