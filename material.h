#ifndef _MATERIAL_
#define _MATERIAL_
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

enum material_type {
	   Tabulated,
	   Function
};

class material {
	    friend class simulation;
		vector <double> raw_wav;
		vector <double> raw_n;
		vector <double> raw_k;
		material_type type;
		string file;
	public:
		material(string);
		double get_n(double);
		double get_k(double);
};
#endif