#ifndef _ELLIPSOMETRY_
#define _ELLIPSOMETRY_
#include "simulation.h"
#include <cstdlib>
using namespace std;

enum dof_type {
	Material,
	Layer
};


class simulation_ellip:public simulation{
	friend class ellipsometry;
		vector<complex<double>>	psi_delta;
		vector<double> psi;
		vector<double> delta;
	public:
		void get_psi_delta(void);	
		void get_psi_delta(string);
		simulation_ellip(ifstream&,double);
};



class DOF {
	 	vector<dof_type> type;
		int idx;
		int length;
		vector<double> parameter_list;
};


class raw_psi_delta {
	friend class ellipsometry;
		vector<double> wav;
		vector<double> psi;
		vector<double> delta;		
};

class ellipsomtry {
// need to use the wavelength data in raw measurement file to overwrite wav_vector
		vector <simulation_ellip*> fitted_data; // different angle fitting result
		vector <raw_psi_delta*> raw_data;       // different angle raw data
		vector <DOF*> fitting_parameters;	// fitting parameters
		double get_error(void);			// get MSE between raw data and fitting data
	public:
		ellipsomtry (ifstream&);
		
};

#endif //_ELLIPSOMETRY_
