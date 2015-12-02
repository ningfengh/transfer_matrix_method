#include "ellipsometry.h"


simulation_ellip::simulation_ellip(ifstream &m_file, double aoi):simulation(m_file){
	this->aoi = aoi;
}

void simulation_ellip::get_psi_delta(void){	
	if (ref_vector_s.size()==0 || ref_vector_p.size()==0) {cout<<"please calculate the reflection first"<<endl;exit(1);}
	for (size_t i=0;i<ref_vector_p.size();i++) {
		complex<double> rho = ref_vector_p[i]/ref_vector_s[i];
		psi_delta.push_back(rho);
		psi.push_back(atan(abs(rho))/M_PI*180.0);
		double delta_tmp = -arg(rho)/M_PI*180.0;
		if (delta_tmp<-90) delta_tmp+=360;
		delta.push_back(delta_tmp);
		
	}
}

void simulation_ellip::get_psi_delta(string filename){	
	ofstream myfile (filename.c_str());
	if (ref_vector_s.size()==0 || ref_vector_p.size()==0) {cout<<"please calculate the reflection first"<<endl;exit(1);}
	for (size_t i=0;i<ref_vector_p.size();i++) {
		complex<double> rho = ref_vector_p[i]/ref_vector_s[i];
		psi_delta.push_back(rho);
		
		psi.push_back(atan(abs(rho))/M_PI*180.0);
		
		double delta_tmp = -arg(rho)/M_PI*180.0;
		if (delta_tmp<-90) delta_tmp+=360;
		delta.push_back(delta_tmp);
		myfile<<wav_vector[i]<<"\t"<<psi[i]<<"\t"<<delta[i]<<endl;
	}
	myfile.close();
}
