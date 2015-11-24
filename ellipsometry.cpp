#include "ellipsometry.h"


simulation_ellip::simulation_ellip(string filename, double aoi):simulation(filename){
	this->aoi = aoi;
}

void simulation_ellip::get_psi_delta(void){	
	if (ref_vector_s.size()==0 || ref_vector_p.size()==0) {cout<<"please calculate the reflection first"<<endl;exit(1);}
	for (int i=0;i<ref_vector_p.size();i++) {
		complex<double> rho = ref_vector_p[i]/ref_vector_s[i];
		psi_delta.push_back(rho);
		psi.push_back(atan(abs(rho))/M_PI*180.0);
		delta.push_back(arg(rho)/M_PI*180.0);
		
	}
}

void simulation_ellip::get_psi_delta(string filename){	
	ofstream myfile (filename.c_str());
	if (ref_vector_s.size()==0 || ref_vector_p.size()==0) {cout<<"please calculate the reflection first"<<endl;exit(1);}
	for (int i=0;i<ref_vector_p.size();i++) {
		complex<double> rho = ref_vector_p[i]/ref_vector_s[i];
		psi_delta.push_back(rho);
		psi.push_back(atan(abs(rho))/M_PI*180.0);
		delta.push_back(arg(rho)/M_PI*180.0);
		myfile<<wav_vector[i]<<"\t"<<psi[i]<<"\t"<<delta[i]<<endl;
	}
	myfile.close();
}