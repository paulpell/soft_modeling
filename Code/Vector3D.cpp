#include "Vector3D.h"

Vector3D::Vector3D(float x, float y, float z) : Vertex3D(x, y, z){
}

Vector3D::~Vector3D() {
	// TODO
}

/*/ Inverse direction of Vector3D
void Vector3D::neg()
{
	x = -x;
	y = -y;
	z = -z;
}

void Vector3D::scale(float s)
{
	x = x*s;
	y = y*s;
	z = z*s;
}

// Combine this Vector3D with another Vector3D
void Vector3D::add(Vector3D * f)
{
	// Vectoraddition (gives direction of combined Vector3D):
	float dx = this->x + f->x;
	float dy = this->y + f->y;
	float dz = this->z + f->z;
	
}*/
