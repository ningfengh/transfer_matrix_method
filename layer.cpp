#include "layer.h"
layer::layer(int idx, double thickness, int variable)
{
	this->idx = idx-1;
	this->thickness = thickness;
	if (variable==1) fitting = true; else fitting = false;
}
