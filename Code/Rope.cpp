#include "Rope.h"

Rope::Rope(){
}

Rope::Rope(MassPoint3D* start){
	// add the startpoints to the pointList
	//list<MassPoint3D*> pointList;
	pointList.push_front(start);

	//springList = list<Spring*>;
}

void Rope::addNode(MassPoint3D* next){
	pointList.push_front(next);

	// Create the connection Springs:
	MassPoint3D* last = pointList.front();
	Spring* spring1 = new Spring(last, next, 0.31);
	spring1->setSize(5);

	// Add Springs to the springList
	springList.push_front(spring1);
}




