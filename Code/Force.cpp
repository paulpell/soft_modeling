#include "Force.h"

Force::Force(float x, float y, float z, float f) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->force = f;
}

Force::Force(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
	//this->force = f;
}

Force::~Force() {
	// TODO
}

// Inverse direction of Force
void Force::neg()
{
	x = -x;
	y = -y;
	z = -z;
}

void Force::scale(float s)
{
	x = x*s;
	y = y*s;
	z = z*s;
}

float Force::getForce(){
	return sqrt(x*x + y*y + z*z);
}

// Combine this Force with another Force
void Force::add(Force * f)
{
	// Vectoraddition (gives direction of combined Force):
	float dx = this->x + f->x;
	float dy = this->y + f->y;
	float dz = this->z + f->z;
	
}
