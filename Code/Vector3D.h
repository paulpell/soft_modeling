// 14.03.2012

#ifndef VECTOR3D_H_
#define VECTOR3D_H_

#include "Vertex3D.h"

class Vector3D : public Vertex3D {
public:
	// Con- and Destructor:
	Vector3D(float x, float y, float z);
	virtual ~Vector3D();
};

class Force : public Vector3D {
public:
	Force(float x, float y, float z);
};

Force::Force(float x, float y, float z) : Vector3D(x, y, z){
}

#endif /* VECTOR3D_H_ */
