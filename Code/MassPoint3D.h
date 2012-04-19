// 14.03.2012

#ifndef MASSPOINT3D_H_
#define MASSPOINT3D_H_

#include <list>

#include "Vertex3D.h"
#include "Vector3D.h"

#include <iostream>

class MassPoint3D : public Vertex3D {
public:
	// Con- und Destructor:
	MassPoint3D();
	MassPoint3D(float x, float y, float z);
	MassPoint3D(float x, float y, float z, float m);
	virtual ~MassPoint3D();



	void addForce(Force *force);
	void setAnchor(bool b);

	void timeStep(float time);


	float old_x, old_y, old_z;
	bool isAnchor;

	float mass;
	Vector3D velocity;

	std::list<Force*> forcelist;

	void draw();
	
};

#endif /* MASSPOINT3D_H_ */
