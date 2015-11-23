#include "simulation.h"

simulation::simulation(string filename)
{
	ifstream m_file (filename.c_str());
	int n_material, n_layer;
	double wav_begin,wav_end,npoint,aoi;
	
	string datafilename;
	
	if(m_file.is_open())
	{
		m_file>>wav_begin>>wav_end>>npoint;
		this->wav_begin = wav_begin;
		this->wav_end = wav_end;
		this->npoint = npoint;
		m_file>>aoi;
		this->aoi = aoi;
		for (int i=0;i<npoint;i++)
		{
			wav_vector.push_back(wav_begin+(wav_end-wav_begin)/(npoint-1)*i);
		}
		
		m_file>>n_material;
		cout<<"number of materials is: "<<n_material<<endl;
		for (int i=0;i<n_material;i++)
		{
			m_file>>datafilename;
			material* temp=new material(datafilename);
			material_data.push_back(temp);
		}

		m_file>>n_layer;
		cout<<endl<<endl<<"number of layer is: "<<n_layer<<endl;
		for (int i=0;i<n_layer;i++)
		{
			double idx,thickness;
			m_file>>idx>>thickness;
			layer* temp = new layer(idx,thickness);
			layer_data.push_back(temp);
			cout<<"layer "<<i+1<< " has the thickness of "<<thickness<<" nm and use the material data in "
			<<material_data[idx-1]->file<<endl;
			
		}

		m_file.close();
	}
	else {cout<<"Unable to open main control file ALL.IN"<<endl;}	
}

void simulation::get_ref_trans(string filename)
{
	if (layer_data.size()<1) {cout<<"please add more than one layer"<<endl;return;}
	complex<double> theta0 (aoi*M_PI/180.0,0.0);
	complex<double> n0 (1.0,0.0);
	complex<double> M[2][2];
	complex <double> EYE (0,1);
	int nlayer = layer_data.size();
	ofstream myfile (filename.c_str());
	
	for (int wav_idx=0;wav_idx<npoint;wav_idx++)
	{
		complex <double> n1(material_data[layer_data[0]->idx]->get_n(wav_vector[wav_idx]),material_data[layer_data[0]->idx]->get_k(wav_vector[wav_idx]));
		complex <double> n2;
		complex <double> theta1 = asin(n0*sin(theta0)/n1);
		complex <double> theta2;
		complex <double> rs = (n0*cos(theta0)-n1*cos(theta1))/(n0*cos(theta0)+n1*cos(theta1));
		complex <double> ts = (2.0*n0*cos(theta0))/(n0*cos(theta0)+n1*cos(theta1));
		M[0][0] = 1.0/ts; M[0][1] = rs/ts;
		M[1][0] = rs/ts;  M[1][1] = 1.0/ts;    // first layer
		
		for (int layer_idx = 0;layer_idx<nlayer-1;layer_idx++)
		{
			n1 = complex <double>(material_data[layer_data[layer_idx]->idx]->get_n(wav_vector[wav_idx]),material_data[layer_data[layer_idx]->idx]->get_k(wav_vector[wav_idx]));
			n2 = complex <double>(material_data[layer_data[layer_idx+1]->idx]->get_n(wav_vector[wav_idx]),material_data[layer_data[layer_idx+1]->idx]->get_k(wav_vector[wav_idx]));
			theta1 = asin(n0*sin(theta0)/n1);
			theta2 = asin(n0*sin(theta0)/n2);
			complex <double> delta = 2.0*M_PI/n0*n1*cos(theta1)/wav_vector[wav_idx]*layer_data[layer_idx]->thickness;		
			rs = (n1*cos(theta1)-n2*cos(theta2))/(n1*cos(theta1)+n2*cos(theta2));
			ts = (2.0*n1*cos(theta1))/(n1*cos(theta1)+n2*cos(theta2));
			complex<double> tmp[2][2];
			tmp[0][0] = M[0][0]*exp(-EYE*delta)/ts+M[0][1]*exp(EYE*delta)*rs/ts;
			tmp[0][1] = M[0][0]*exp(-EYE*delta)*rs/ts+M[0][1]*exp(EYE*delta)/ts;
			tmp[1][0] = M[1][0]*exp(-EYE*delta)/ts+M[1][1]*exp(EYE*delta)*rs/ts;
			tmp[1][1] = M[1][0]*exp(-EYE*delta)*rs/ts+M[1][1]*exp(EYE*delta)/ts;	
			M[0][0] = tmp[0][0];
			M[0][1] = tmp[0][1];
			M[1][0] = tmp[1][0];
			M[1][1] = tmp[1][1];
				
		}
		n1 = complex <double>(material_data[layer_data[nlayer-1]->idx]->get_n(wav_vector[wav_idx]),material_data[layer_data[nlayer-1]->idx]->get_k(wav_vector[wav_idx]));
		theta1 = asin(n0*sin(theta0)/n1);
		complex <double> delta = 2.0*M_PI/n0*n1*cos(theta1)/wav_vector[wav_idx]*layer_data[nlayer-1]->thickness;		
		rs = (n1*cos(theta1)-n0*cos(theta0))/(n1*cos(theta1)+n0*cos(theta0));
		ts = (2.0*n1*cos(theta1))/(n1*cos(theta1)+n0*cos(theta0));
		complex<double> tmp[2][2];
		tmp[0][0] = M[0][0]*exp(-EYE*delta)/ts+M[0][1]*exp(EYE*delta)*rs/ts;
		tmp[0][1] = M[0][0]*exp(-EYE*delta)*rs/ts+M[0][1]*exp(EYE*delta)/ts;
		tmp[1][0] = M[1][0]*exp(-EYE*delta)/ts+M[1][1]*exp(EYE*delta)*rs/ts;
		tmp[1][1] = M[1][0]*exp(-EYE*delta)*rs/ts+M[1][1]*exp(EYE*delta)/ts;
		complex<double> ref = tmp[1][0]/tmp[0][0];
		complex<double> trans = 1.0/tmp[0][0];
		ref_vector.push_back(norm(ref));
		trans_vector.push_back(norm(trans));
		myfile<<wav_vector[wav_idx]<<"\t"<<norm(ref)<<"\t"<<norm(trans)<<endl;
	}
	myfile.close();
}