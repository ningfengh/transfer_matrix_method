#include <iostream>
#include <fstream>
#include <vector>

//#include "ellipsometry.h"
#include "simulation.h"
using namespace std;

int main(void)
{
	
	// TEST TRANSMISSION AND REFLECTION
	simulation mysimulation("ALL.IN");
	mysimulation.get_ref_trans("myout_s.txt",'s');
	mysimulation.get_ref_trans("myout_p.txt",'p');
	return 1;
	
/*
	material mymaterial("TCO_fit.txt");
	
	for (double wav=300;wav<1000;wav+=0.1)
	{
		cout<<wav<<"\t"<<real(mymaterial.get_nk(wav))<<"\t"<<imag(mymaterial.get_nk(wav))<<endl;
	}
	*/
	/*
	simulation_ellip mysimulation("ALL.IN",55);
	mysimulation.get_ref_trans("myout_s.txt",'s');
	mysimulation.get_ref_trans("myout_p.txt",'p');
	mysimulation.get_psi_delta("psi_delta.txt");
*/
}
