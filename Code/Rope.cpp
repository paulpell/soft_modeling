#include "Rope.h"

Rope::Rope(){
}

Rope::Rope(float x, float y, float z, float dx, float dy, float dz){

	// TODO more than one "segment"

	MassPoint3D start(x, y, z);
	MassPoint3D end(dx, dy, dz);

	// TODO anchorpoint possible at other or multiple positions...
	start.setAnchor(true);

	// add the Points to the pointList
	//pointList = list<MassPoint3D>;
	pointList.push_front(start);
	pointList.push_front(end);

	// Create the connection Springs:
	Spring spring1(&start, &end, 1);

	// Add Springs to the springList
	springList.push_front(spring1);

}

void Rope::draw(){

	//list<Spring>::iterator it, listend = springList.end();
	//for (it = springList.begin(); it != listend; it++) {
	//	(*it).draw();
	//}

}
