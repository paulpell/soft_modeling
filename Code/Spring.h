// 14.03.2012

#ifndef SPRING_H_
#define SPRING_H_

#include "MassPoint3D.h"

#include <math.h> // For sqrt()

    using namespace std;

class Spring {

public:
	Spring(); // TODO hmmm?

	Spring(MassPoint3D *start, MassPoint3D *end, float strength);

	virtual ~Spring();


	// void addMassPoint(MassPoint3D p);

	void timeStep(float time);

	// Graphical representation:
	void setSize(int s);
	void draw();

	float getLength();



	MassPoint3D *start, *end;
	float org_length, hardness;

	Force *intForceE, *intForceS; // for End and Start

	int size;
};

#endif /* SPRING_H_ */
