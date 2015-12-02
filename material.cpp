#include "material.h"

material_model::material_model(fit_model mymodel,vector<double>& myparameter)
{
	model = mymodel;
	parameters = myparameter;
}


void material_model::set_parameter(vector<double>& parameters){
	if (model==Offset){	
		if (parameters.size()!=1) {cout<<"The Offset model should have one parameter"<<endl;exit(1);}
		else this->parameters = parameters;
	}
	else if (model==Lorentz){
		if (parameters.size()!=3) {cout<<"The Lorentz model should have three parameters"<<endl;exit(1);}
		else this->parameters = parameters;
	}
	else if (model==Drude){
		if (parameters.size()!=2) {cout<<"The Drude model should have two parameters"<<endl;exit(1);}
		else this->parameters = parameters;		
	}
}


complex<double> material_model::get_nk(double wav_in)
{
	complex<double> EYE(0.0,1.0);
	if (model==Offset){	
		if (parameters.size()!=1) {cout<<"The Offset model should have one parameter"<<endl;exit(1);}
		else return complex<double>(parameters[0],0);
	}
	else if (model==Lorentz){
		if (parameters.size()!=3) {cout<<"The Lorentz model should have three parameters"<<endl;exit(1);}
		else return parameters[0]/(parameters[1]*parameters[1]-(1241.0/wav_in)*(1241.0/wav_in)-EYE*parameters[2]*1241.0/wav_in);
	}
	else if (model==Drude){
		if (parameters.size()!=2) {cout<<"The Drude model should have two parameters"<<endl;exit(1);}
		else return -parameters[0]/((1241.0/wav_in)*(1241.0/wav_in)+EYE*parameters[1]*1241.0/wav_in);
	}
        return 0;
}

material::material(string filename, int variable){
	ifstream material_in(filename.c_str());
	string material_type;
	file = filename;
	if (material_in.is_open()) {
		material_in>>material_type;
		if (material_type=="Table"){
			if (variable==1) {cout<<"[Error] Tabulated material data cannot be variable..."<<endl;exit(1);}
			fitting = false;
			type = Tabulated;
			double wav,n,k;		
			while (material_in>>wav)
			{				
				
				material_in>>n;
				material_in>>k;
				
				raw_wav.push_back(wav);
			//[to do], decide whether wav is monotonically increase
				raw_n.push_back(n);
				raw_k.push_back(k);
			}
			cout<<raw_wav.size()<<" data points have been read from "<<filename<<
			" from "<<raw_wav[0]<<" nm to "<<raw_wav[raw_wav.size()-1]<<" nm"<<endl;
			material_in.close();
		}
		else if (material_type=="Model"){
			if (variable==1) fitting = true; else fitting = false;			
			type = Model;
			while (!material_in.eof())
			{
				string model_type;
				double parameter;
				material_in>>model_type;
				if (model_type=="Offset"){
					vector<double> parameters;
					material_in>>parameter;
					parameters.push_back(parameter);
					material_model* temp = new material_model(Offset,parameters);
					models.push_back(temp);
				}
				else if (model_type=="Drude"){
					vector<double> parameters;
					material_in>>parameter;
					parameters.push_back(parameter);
					material_in>>parameter;
					parameters.push_back(parameter);	
					material_model* temp = new material_model(Drude,parameters);
					models.push_back(temp);					
				}
				else if (model_type=="Lorentz") {
					vector<double> parameters;
					material_in>>parameter;
					parameters.push_back(parameter);
					material_in>>parameter;
					parameters.push_back(parameter);	
					material_in>>parameter;
					parameters.push_back(parameter);
					material_model* temp = new material_model(Lorentz,parameters);
					models.push_back(temp);						
				}
				
			}
		}
		
	}
	else {
		cout<<"Unable to open material file "<<filename<<endl;
	}
}


complex<double> material::get_nk(double wav_in){
	if (type==Tabulated){	
		if (raw_wav.size()==0) cout<<"Please initialize material first!"<<endl;
		else if (wav_in<=raw_wav[0]) return complex<double>(raw_n[0],raw_k[0]);
		else if (wav_in>=raw_wav[raw_wav.size()-1]) return complex<double>(raw_n[raw_wav.size()-1],raw_k[raw_wav.size()-1]);
		else {//binary search, linear interpolateion
		
			int begin = 0, end = raw_wav.size()-1;
			int mid = (begin+end)/2;
			while (begin<end-1){			
				if (wav_in == raw_wav[mid]) return complex<double>(raw_n[mid],raw_k[mid]);
				else if (wav_in>raw_wav[mid]) {begin = mid;mid = (begin+end)/2;}
				else {end = mid;mid=(begin+end)/2;}
			}
			return complex<double> (raw_n[begin] + (raw_n[end]-raw_n[begin])/(raw_wav[end]-raw_wav[begin])*(wav_in-raw_wav[begin]),
									raw_k[begin] + (raw_k[end]-raw_k[begin])/(raw_wav[end]-raw_wav[begin])*(wav_in-raw_wav[begin]));
		}
	}
	else if (type==Model){
		complex<double>eps(0.0,0.0);
		for (size_t i=0;i<models.size();i++){
			eps+=models[i]->get_nk(wav_in);
		}
		return sqrt(eps);		
	}
	return 0;
}


