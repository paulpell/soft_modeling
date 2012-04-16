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

	//Rope(MassPoint3D* start);

	void addNode(MassPoint3D* next);

	void reset();

	void draw();

	// sry nicht nice
	float veclength(float x1, float y1, float z1);
	float vecdot(float x1, float y1, float z1, float x2, float y2, float z2);
	float vecangle(float x1, float y1, float z1, float x2, float y2, float z2);

private:

	// 
	int segments, segsize, hardness, radius;

};

#endif
