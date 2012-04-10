// 15.03.2012

#ifndef ROPE_H_
#define ROPE_H_

#include "WorldObject.h"
#include "MassPoint3D.h"
#include "Spring.h"

#include <list>

class Rope : public WorldObject {
public:
	// Con- and Destructor:
	Rope();
	Rope(MassPoint3D* start);

	void addNode(MassPoint3D* next);


};

#endif
