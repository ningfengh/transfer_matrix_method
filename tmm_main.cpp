#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include "ellipsometry.h"
//#include "simulation.h"
using namespace std;

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
		simulation_ellip mysimulation(m_file,55);
		mysimulation.get_ref_trans("./output/spec_s.txt",'s');
		mysimulation.get_ref_trans("./output/spec_p.txt",'p');
		mysimulation.get_psi_delta("./output/psi_delta.txt");
	}	
	return 1;

	
	m_file.close();

}
