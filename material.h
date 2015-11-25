#ifndef _MATERIAL_
#define _MATERIAL_
#include <iostream>
#include <fstream>
#include <vector>
#include <complex>
#include <cstdlib>
using namespace std;

enum material_type {
	   Tabulated,
	   Model
};

enum fit_model {
	   Offset,
	   Lorentz,
	   Drude
};

class material_model {
		fit_model model;
		vector<double> parameters;
		public:			
			complex<double> get_nk(double);	
			void set_parameter(vector<double>&);	
			material_model(fit_model,vector<double>&);
};


class material {
	    friend class simulation;
		material_type type;
		vector <double> raw_wav;
		vector <double> raw_n;
		vector <double> raw_k;
		vector <material_model*> models;
		string file;
	public:
		material(string); //constructor from a control file
		//set_parameter(int,vector<double>&); // first argument is the idx of the model
		complex<double> get_nk(double);		
};
#endif
