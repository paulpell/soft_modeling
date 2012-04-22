#include "Force.h"

Force::Force() {
}

Force::Force(float x, float y, float z) : Vector3D(x,y,z) {
}

Force::~Force() {
}

// Inverse direction of Force
void Force::neg()
{
	x = -x;
	y = -y;
	z = -z;
}

void Force::reset() {
    x = y = z = 0;
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
