// 15.03.2012

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

	// Properties:
	list<Spring*> springList;
	list<MassPoint3D*> pointList;
    //list<Spring> springList;
    //list<MassPoint3D> pointList;
};

#endif /* WORLDOBJECT_H_ */
