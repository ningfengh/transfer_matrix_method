#include "ellipsometry.h"


simulation_ellip::simulation_ellip(ifstream &m_file):simulation(m_file){
	
}

void simulation_ellip::get_psi_delta(void){	
	if (ref_vector_s.size()==0 || ref_vector_p.size()==0) {cout<<"please calculate the reflection first"<<endl;exit(1);}
	psi_delta.clear();	
	psi.clear();
	delta.clear();
	for (size_t i=0;i<ref_vector_p.size();i++) {
		complex<double> rho = ref_vector_p[i]/ref_vector_s[i];
		psi_delta.push_back(rho);
		psi.push_back(atan(abs(rho))/M_PI*180.0);
		double delta_tmp = -arg(rho)/M_PI*180.0;
		if (delta_tmp<-90) delta_tmp+=360;
		delta.push_back(delta_tmp);
		
	}
}

simulation_ellip::simulation_ellip(void){
}

void simulation_ellip::get_psi_delta(string filename){	
	ofstream myfile (filename.c_str());
	if (ref_vector_s.size()==0 || ref_vector_p.size()==0) {cout<<"please calculate the reflection first"<<endl;exit(1);}
	psi_delta.clear();	
	psi.clear();
	delta.clear();
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

ellipsometry::ellipsometry(ifstream &m_file){
	int n_material, n_layer;
	string filename;
	double wav_begin,wav_end,npoint,aoi;
	simulation_ellip* tmp_sim = new simulation_ellip(m_file);
	fitted_data.push_back(tmp_sim);
	int n_raw;
	m_file>>n_raw;
	cout<<endl<<endl<<"number of ellipsometry raw data is: "<<n_raw<<endl;
	for (size_t i=0;i<n_raw;i++){
		m_file>>filename;
		double aoi;
		raw_psi_delta* tmp_raw = new raw_psi_delta(filename,tmp_sim->wav_begin,tmp_sim->wav_end);
		raw_data.push_back(tmp_raw);
		m_file>>aoi;
		tmp_raw->aoi = aoi;
		if (i==0) {
			fitted_data[i]->override_aoi(aoi);
			fitted_data[i]->override_wav(tmp_raw->wav_vector.size(),tmp_raw->wav_vector);
		}
		else {
			simulation_ellip* tmp_sim = new simulation_ellip();
			*tmp_sim = *fitted_data[0];
			fitted_data.push_back(tmp_sim);
			fitted_data[i]->override_aoi(aoi);
			fitted_data[i]->override_wav(tmp_raw->wav_vector.size(),tmp_raw->wav_vector);
		}
	}
	cout<<endl<<endl;
	for (size_t i=0;i<fitted_data[0]->layer_data.size();i++)
	{
		if (fitted_data[0]->layer_data[i]->is_variable())
		{
			cout<<"Add the thickness of layer "<<i+1<<" as a fitting parameter"<<endl;
			vector<double> tmp(1,fitted_data[0]->layer_data[i]->get_thickness());
			DOF* tmp_dof = new DOF(Layer,i,tmp);
			fitting_parameters.push_back(tmp_dof);
			parameters.push_back(fitted_data[0]->layer_data[i]->get_thickness());
			
		}
	}		


}


void ellipsometry::delinearize_parameter(void){
	fitting_parameters[0]->parameters[0] = parameters[0];
}


double ellipsometry::myfunc(const std::vector<double> &x, std::vector<double> &grad, void *data)
{
	
	parameters = x;
	delinearize_parameter();

	for (size_t i = 0; i<fitting_parameters.size();i++) {
		if (fitting_parameters[i]->type==Layer){
			for (size_t j=0;j<fitted_data.size();j++){
				fitted_data[j]->layer_data[fitting_parameters[i]->idx]->set_thickness(fitting_parameters[i]->parameters[0]);
			}
		}	
	}
	for (size_t i = 0; i<fitted_data.size();i++) {
		fitted_data[i]->get_ref_trans('s');
		fitted_data[i]->get_ref_trans('p');
		fitted_data[i]->get_psi_delta();
	}
	return get_error();
}


double ellipsometry::get_error()
{
	double error = 0;

	for (size_t i = 0 ;i<fitted_data.size();i++) {
		for (size_t j = 0; j<raw_data[i]->wav_vector.size(); j++) {
			error += (raw_data[i]->psi[j]-fitted_data[i]->psi[j])*(raw_data[i]->psi[j]-fitted_data[i]->psi[j]);
			error += (raw_data[i]->delta[j]-fitted_data[i]->delta[j])*(raw_data[i]->delta[j]-fitted_data[i]->delta[j]);
		}	
	}
	return error;
	
}

void ellipsometry::fitting(void){
	cout<<"enter"<<endl;	
	vector<double> x(1,1.4785);
	vector<double> grad;
	for (double t=1;t<2;t+=0.01)
	{
		x[0] =t;
		cout<<t<<" "<<myfunc(x, grad, NULL)<<endl;
	}

}



raw_psi_delta::raw_psi_delta(string filename,double begin,double end) {
	ifstream data_in(filename.c_str());
	if (!data_in.is_open()) {
		cout<<"Cannot open file: "<<filename<<endl;
		exit(1);
	}
	else
	{
		double wav,psi,delta;		
		while (data_in>>wav)
		{				
			data_in>>psi;
			data_in>>delta;
			
			if (wav>=begin && wav<=end)
			{	
				wav_vector.push_back(wav);
				this->psi.push_back(psi);
				this->delta.push_back(delta);
			}
		}
		cout<<wav_vector.size()<<" data points have been read from "<<filename<<
		" from "<<wav_vector[0]<<" nm to "<<wav_vector[wav_vector.size()-1]<<" nm"<<endl;
		data_in.close();
	}
}

DOF::DOF(dof_type type, int idx, vector<double> parameters)
{
	this->type = type;
	this->idx = idx;
	this->parameters = parameters;
}

