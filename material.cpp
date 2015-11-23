#include "material.h"

material::material(string filename){
	ifstream material_in(filename.c_str());
	type = Tabulated;
	if (material_in.is_open()) {
		while (!material_in.eof())
		{
			double wav,n,k;
			material_in>>wav;
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
	else {
		cout<<"Unable to open material file "<<filename<<endl;
	}
}

double material::get_n(double wav_in){
	if (raw_wav.size()==0) cout<<"Please initialize material first!"<<endl;
	else if (wav_in<=raw_wav[0]) return raw_n[0];
	else if (wav_in>=raw_wav[raw_wav.size()-1]) return raw_n[raw_wav.size()-1];
	else //binary search, linear interpolateion
	{
		int begin = 0, end = raw_wav.size()-1;
		int mid = (begin+end)/2;
		while (begin<end-1)
		{
			if (wav_in == raw_wav[mid]) return raw_n[mid];
			else if (wav_in>raw_wav[mid]) {begin = mid;mid = (begin+end)/2;}
			else {end = mid;mid=(begin+end)/2;}
		}
		return raw_n[begin] + (raw_n[end]-raw_n[begin])/(raw_wav[end]-raw_wav[begin])*(wav_in-raw_wav[begin]);
	}
}

double material::get_k(double wav_in){
	if (raw_wav.size()==0) cout<<"Please initialize material first!"<<endl;
	else if (wav_in<=raw_wav[0]) return raw_k[0];
	else if (wav_in>=raw_wav[raw_wav.size()-1]) return raw_k[raw_wav.size()-1];
	else //binary search
	{
		int begin = 0, end = raw_wav.size()-1;
		int mid = (begin+end)/2;
		while (begin<end-1)
		{
			if (wav_in == raw_wav[mid]) return raw_k[mid];
			else if (wav_in>raw_wav[mid]) {begin = mid;mid = (begin+end)/2;}
			else {end = mid;mid=(begin+end)/2;}
		}
		return raw_k[begin] + (raw_k[end]-raw_k[begin])/(raw_wav[end]-raw_wav[begin])*(wav_in-raw_wav[begin]);
	}
}
