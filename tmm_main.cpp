#include <iostream>
#include <fstream>
#include <vector>
#include "material.h"

using namespace std;

int main(void)
{
	ifstream m_file ("ALL.IN");
	int n_material, n_layer;
	string filename;
	vector <material*> material_data;
	vector <layer*> layer_data;
	if(m_file.is_open())
	{
		m_file>>n_material;
		cout<<"number of materials is: "<<n_material<<endl;
		for (int i=0;i<n_material;i++)
		{
			m_file>>filename;
			material* temp=new material(filename);
			material_data.push_back(temp);
		}

		m_file>>n_layer;
		cout<<"number of layer is: "<<n_layer<<endl;
		for (int i=0;i<n_layer;i++)
		{
			double idx,thickness;
			m_file>>idx>>thickness;
			layer* temp = new layer(idx,thickness);
			layer_data.push_back(temp);
		}

		m_file.close();

		
	}
	else {cout<<"Unable to open main control file ALL.IN"<<endl;}
	
}
