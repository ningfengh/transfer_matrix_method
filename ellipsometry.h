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
		simulation_ellip(ifstream&);
		simulation_ellip(void);
};



class DOF {
	friend class ellipsometry;
	 	dof_type type;
		int idx;
		vector<double> parameters;
	public:
		DOF(dof_type, int, vector<double>);
};


class raw_psi_delta {
	friend class ellipsometry;
		vector<double> wav_vector;
		vector<double> psi;
		vector<double> delta;	
		double aoi;	
	public:
		raw_psi_delta(string,double,double);
};

class ellipsometry {
// need to use the wavelength data in raw measurement file to overwrite wav_vector
		vector <simulation_ellip*> fitted_data; // different angle fitting result
		vector <raw_psi_delta*> raw_data;       // different angle raw data
		vector <DOF*> fitting_parameters;	// fitting parameters
		double get_error(void);			// get MSE between raw data and fitting data
		vector <double> parameters;
		double myfunc(const std::vector<double> &, std::vector<double> &, void *);
		void delinearize_parameter();

	public:
		ellipsometry (ifstream&);
		
		void fitting();
		
		
};

#endif //_ELLIPSOMETRY_
