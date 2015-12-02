#include "simulation.h"

simulation::simulation(ifstream &m_file)   // constructor from the control file
{
	int n_material, n_layer;
	double wav_begin,wav_end,npoint,aoi;
	
	string datafilename;
	
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
		
//      Read material files
	m_file>>n_material;
	cout<<"number of materials is: "<<n_material<<endl;
	for (int i=0;i<n_material;i++)
	{
		m_file>>datafilename;
		material* temp=new material(datafilename); // class material constructer takes filename as an input
		material_data.push_back(temp);
	}

//      Read layers info, the last layer is always semi-infinite substrate layer

	m_file>>n_layer;
	cout<<endl<<endl<<"number of layer is: "<<n_layer<<endl;
	for (int i=0;i<n_layer-1;i++)
	{
		double idx,thickness;
		m_file>>idx>>thickness;
		layer* temp = new layer(idx,thickness);  // idx is material indice, thickness is the only parameter for each layer
		layer_data.push_back(temp);
		cout<<"layer "<<i+1<< " has the thickness of "<<thickness<<" nm and use the material data in "
		<<material_data[idx-1]->file<<endl;
	}
	double idx,thickness;
	m_file>>idx>>thickness;
	if (thickness!=-1) {cout<<"last layer is semi-infinite, put thickness as -1\n"<<endl;exit(1);}
	layer* temp = new layer(idx,thickness);
	layer_data.push_back(temp);
	cout<<"layer "<<n_layer<< " is semi-infinite substrate and use the material data in "
	<<material_data[idx-1]->file<<endl;
	m_file.close();

}

void simulation::get_ref_trans(string filename, char s)  // always have the last semi-infinite substrate layer
{
	if (layer_data.size()<1) {cout<<"please add more than one layer"<<endl;return;}
	complex	<double> theta0 (aoi*M_PI/180.0,0.0);
	complex	<double> n0 (1.0,0.0);
	complex	<double> M[2][2];
	complex	<double> tmp[2][2];
	complex <double> EYE (0,1);
	complex <double> n1, n2;
	complex <double> theta1, theta2;
	complex <double> rs, ts;
	complex <double> delta;
	complex	<double> ref, trans;
	
	int nlayer = layer_data.size();
	ofstream myfile (filename.c_str());
	
	if (s=='s'){
	
		for (int wav_idx=0;wav_idx<npoint;wav_idx++)
		{
			/* calculate the first layer matrix */
			n1 = material_data[layer_data[0]->idx]->get_nk(wav_vector[wav_idx]);
			theta1 = asin(n0*sin(theta0)/n1);
			rs = (n0*cos(theta0)-n1*cos(theta1))/(n0*cos(theta0)+n1*cos(theta1));
			ts = (2.0*n0*cos(theta0))/(n0*cos(theta0)+n1*cos(theta1));
			M[0][0] = 1.0/ts; M[0][1] = rs/ts;
			M[1][0] = rs/ts;  M[1][1] = 1.0/ts;    
		
		
			/* calculate the middle layers */
			for (int layer_idx = 0;layer_idx<nlayer-1;layer_idx++)
			{
				n1 = material_data[layer_data[layer_idx]->idx]->get_nk(wav_vector[wav_idx]);
				n2 = material_data[layer_data[layer_idx+1]->idx]->get_nk(wav_vector[wav_idx]);
				theta1 = asin(n0*sin(theta0)/n1);
				theta2 = asin(n0*sin(theta0)/n2);
				delta = 2.0*M_PI/n0*n1*cos(theta1)/wav_vector[wav_idx]*layer_data[layer_idx]->thickness;		
				rs = (n1*cos(theta1)-n2*cos(theta2))/(n1*cos(theta1)+n2*cos(theta2));
				ts = (2.0*n1*cos(theta1))/(n1*cos(theta1)+n2*cos(theta2));
			
				tmp[0][0] = M[0][0]*exp(-EYE*delta)/ts+M[0][1]*exp(EYE*delta)*rs/ts;
				tmp[0][1] = M[0][0]*exp(-EYE*delta)*rs/ts+M[0][1]*exp(EYE*delta)/ts;
				tmp[1][0] = M[1][0]*exp(-EYE*delta)/ts+M[1][1]*exp(EYE*delta)*rs/ts;
				tmp[1][1] = M[1][0]*exp(-EYE*delta)*rs/ts+M[1][1]*exp(EYE*delta)/ts;
			
				M[0][0] = tmp[0][0];
				M[0][1] = tmp[0][1];
				M[1][0] = tmp[1][0];
				M[1][1] = tmp[1][1];
				
			}	
		
					
			ref = M[1][0]/M[0][0];
			trans = 1.0/M[0][0];
		
			ref_vector_s.push_back(ref);
			trans_vector_s.push_back(trans);
		
			myfile<<wav_vector[wav_idx]<<"\t"<<norm(ref)<<"\t"<<norm(trans)<<endl;
		}
	}
	else if (s=='p'){
		for (int wav_idx=0;wav_idx<npoint;wav_idx++)
		{
			/* calculate the first layer matrix */
			n1 = material_data[layer_data[0]->idx]->get_nk(wav_vector[wav_idx]);
			
			theta1 = asin(n0*sin(theta0)/n1);
			rs = (n1*cos(theta0)-n0*cos(theta1))/(n1*cos(theta0)+n0*cos(theta1));
			ts = (2.0*n0*cos(theta0))/(n1*cos(theta0)+n0*cos(theta1));
			M[0][0] = 1.0/ts; M[0][1] = rs/ts;
			M[1][0] = rs/ts;  M[1][1] = 1.0/ts;    
		
		
			/* calculate the middle layers */
			for (int layer_idx = 0;layer_idx<nlayer-1;layer_idx++)
			{
				n1 = material_data[layer_data[layer_idx]->idx]->get_nk(wav_vector[wav_idx]);
				n2 = material_data[layer_data[layer_idx+1]->idx]->get_nk(wav_vector[wav_idx]);
				theta1 = asin(n0*sin(theta0)/n1);
				theta2 = asin(n0*sin(theta0)/n2);
				delta = 2.0*M_PI/n0*n1*cos(theta1)/wav_vector[wav_idx]*layer_data[layer_idx]->thickness;		
				rs = (n2*cos(theta1)-n1*cos(theta2))/(n2*cos(theta1)+n1*cos(theta2));
				ts = (2.0*n1*cos(theta1))/(n2*cos(theta1)+n1*cos(theta2));
			
				tmp[0][0] = M[0][0]*exp(-EYE*delta)/ts+M[0][1]*exp(EYE*delta)*rs/ts;
				tmp[0][1] = M[0][0]*exp(-EYE*delta)*rs/ts+M[0][1]*exp(EYE*delta)/ts;
				tmp[1][0] = M[1][0]*exp(-EYE*delta)/ts+M[1][1]*exp(EYE*delta)*rs/ts;
				tmp[1][1] = M[1][0]*exp(-EYE*delta)*rs/ts+M[1][1]*exp(EYE*delta)/ts;
			
				M[0][0] = tmp[0][0];
				M[0][1] = tmp[0][1];
				M[1][0] = tmp[1][0];
				M[1][1] = tmp[1][1];
				
			}	
		
					
			ref = M[1][0]/M[0][0];
			trans = 1.0/M[0][0];
		
			ref_vector_p.push_back(ref);
			trans_vector_p.push_back(trans);
		
			myfile<<wav_vector[wav_idx]<<"\t"<<norm(ref)<<"\t"<<norm(trans)<<endl;
		}		
	}
	else cout<<"Please select the correct polarization"<<endl;
	myfile.close();
}
