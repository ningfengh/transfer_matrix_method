#include <iostream>
#include <fstream>
#include <vector>

#include "simulation.h"
using namespace std;

int main(void)
{
	simulation mysimulation("ALL.IN");
	mysimulation.get_ref_trans("myout.txt");
	return 1;	
}
