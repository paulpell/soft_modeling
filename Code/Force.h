// 14.03.2012
#pragma once

#ifndef FORCE_H_
#define FORCE_H_

#include "Vector3D.h"
#include <math.h>

class Force: public Vector3D {
public:
	// Con- and Destructor:
    Force();
	Force(float x, float y, float z);
	virtual ~Force();

	// Arithmetic methods:
	void neg();
    void reset();// sets to 0
	void scale(float s);
	void add(Force * f);		

	// Properties:
	float getForce();

};

#endif /* FORCE_H_ */
