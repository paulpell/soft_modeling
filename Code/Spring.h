// 14.03.2012

#ifndef SPRING_H_
#define SPRING_H_

#include "MassPoint3D.h"

#include <math.h> // For sqrt()

using namespace std;

class Spring {

public:
	Spring();
	Spring(MassPoint3D *start, MassPoint3D *end, float strength);
	virtual ~Spring();

	void timeStep(float time);

	// Graphical representation:
	void draw();

	float getLength();

	MassPoint3D *start, *end;
	float org_length, hardness;

	// internal Forces, one for EndMass one for StartMass
	Force *intForceE, *intForceS;

};

#endif /* SPRING_H_ */
