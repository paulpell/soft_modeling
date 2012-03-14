// 14.03.2012

#ifndef FORCE_H_
#define FORCE_H_

#include "Force.h"

class Force {
public:
	// Con- and Destructor:
	Force(float x, float y, float z, float f);
	virtual ~Force();

	// Arithmetic methods:
	void neg();
	void scale(float s);
	void add(Force * f);		

	// Properties:
	float force;	// amount
	float x, y, z;  // direction

};

#endif /* FORCE_H_ */
