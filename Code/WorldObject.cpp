#include "WorldObject.h"

WorldObject::WorldObject(){
}

WorldObject::~WorldObject() {
}

void WorldObject::timeStep(float time){
    
	cout << "new timestep " << endl << endl << endl;

	// slows down [not by reducing time, BUT reducing forces!]
	time = time / .7;

	// update all the internal spring forces
	list<Spring*>::iterator it, end = springList.end();
	for (it = springList.begin(); it != end; it++) {
		(*it)->timeStep(time);
	}

	// update all the Masspoints
	list<MassPoint3D*>::iterator it2, end2 = pointList.end();
	for (it2 = pointList.begin(); it2 != end2; it2++) {
		(*it2)->timeStep(time);
	}

}

void WorldObject::draw(){
	list<Spring*>::iterator it, listend = springList.end();
	for (it = springList.begin(); it != listend; it++) {
	        (*it)->draw();
	}
}

void WorldObject::applyGlobalForce( Force *f) {

	//list<MassPoint3D>::iterator it, end = pointList.end();
	list<MassPoint3D*>::iterator it, end = pointList.end();
	for (it = pointList.begin(); it != end; it++) {
	//(*it).addForce(f);
	(*it)->addForce(f);
	}
}

void WorldObject::pushSpring(MassPoint3D* start, MassPoint3D* end, float hardness){
	// Create the connection Springs:
	Spring* spring1 = new Spring(start, end, hardness);
	//spring1->setSize(1); // remove later

	// Add Springs to the springList
	springList.push_front(spring1);
}


