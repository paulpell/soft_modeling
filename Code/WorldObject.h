// 15.03.2012

#ifndef WORLDOBJECT_H_
#define WORLDOBJECT_H_

#include "Spring.h"
#include "MassPoint3D.h"

#include <list>

class WorldObject { // abstract
public:

	// Con- and Destructor:
	WorldObject();
	virtual ~WorldObject();


	// 
	void applyGlobalForce(Force *force);

	// 
	void draw();

	// Properties:
	std::list<Spring> springList;
	std::list<MassPoint3D> pointList;
};

#endif /* WORLDOBJECT_H_ */
