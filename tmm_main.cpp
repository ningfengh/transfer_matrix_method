#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include "ellipsometry.h"
//#include "simulation.h"
using namespace std;

double wrapper(const std::vector<double> &x, std::vector<double> &grad, void *o) {
	return reinterpret_cast<ellipsometry*>(o)->myfunc(x, grad, o);
}



int main(int nNumberofArgs, char* pszArgs[])
{
	string simulation_type;
	if (nNumberofArgs!=2) {cout<<"Please specify the control file..."<<endl;exit(1);}
	ifstream m_file (pszArgs[1]);
	if(!m_file.is_open()) {cout<<"Error: cannot open the control file..."<<endl;exit(1);}

	
	m_file>>simulation_type;
	if (simulation_type=="Spec")
	{
		simulation mysimulation(m_file);
		mysimulation.get_ref_trans("./output/spec_s.txt",'s');
		mysimulation.get_ref_trans("./output/spec_p.txt",'p');
	}
	else if(simulation_type=="Ellip")
	{
		simulation_ellip mysimulation(m_file);
		mysimulation.get_ref_trans("./output/spec_s.txt",'s');
		mysimulation.get_ref_trans("./output/spec_p.txt",'p');
		mysimulation.get_psi_delta("./output/psi_delta.txt");
	}
	else if(simulation_type=="Fitting")
	{
		ellipsometry mysimulation(m_file);
		vector<double> x(1,1.4785);
		vector<double> grad;
		for (double t=1;t<2;t+=0.01)
		{
			x[0] =t;
			cout<<t<<" "<<wrapper(x, grad, &mysimulation)<<endl;
		}
		//mysimulation.fitting();
		//mysimulation.get_ref_trans("./output/spec_s.txt",'s');
		//mysimulation.get_ref_trans("./output/spec_p.txt",'p');
		//mysimulation.get_psi_delta("./output/psi_delta.txt");
	}		
	return 1;

	
	m_file.close();

}
