//============================================================================
// Name        : 
// Author      : Paul and Philipp
// Version     : 15.03.2012
// Description : Modeling Soft Objects with OpenGL
//============================================================================

#ifndef WORLDOBJECT_H_
#define WORLDOBJECT_H_

#include "Spring.h"
#include "MassPoint3D.h"

#include <list>

using namespace std;

class WorldObject { // abstract
public:

	// Con- and Destructor:
	WorldObject();
	virtual ~WorldObject();

	// 
	void applyGlobalForce(Force *force);

	// 
	void draw();

	// 
	void timeStep(float time);

	void pushSpring(MassPoint3D* start, MassPoint3D* end, float hardness);

	// for Collision detection - returns a set of MP at z=0
	//MassPoint3D* getImpactSet();

// private:
	// Properties:
	list<Spring*> springList;
	list<MassPoint3D*> pointList;

};

#endif /* WORLDOBJECT_H_ */
