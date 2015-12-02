#include "layer.h"
layer::layer(int idx, double thickness, int variable)
{
	this->idx = idx-1;
	this->thickness = thickness;
	if (variable==1) fitting = true; else fitting = false;
}

bool layer::is_variable(void){
	return fitting;
}

double layer::get_thickness(void){
	return thickness;
}

void layer::set_thickness(double thickness){
	this->thickness = thickness;
}
