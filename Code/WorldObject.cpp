#include "WorldObject.h"

WorldObject::WorldObject(){
    containsObject = false;
}

WorldObject::~WorldObject() {
}

void WorldObject::timeStep(float time){
    

	// slows down [not by reducing time, BUT reducing forces!]
	time = time / .7;

    // update the sub-objects
    if (containsObject) {
        list<WorldObject*>::iterator itO, endO = objects.end();
        for (itO = objects.begin(); itO != endO; itO++) {
            (*itO)->timeStep(time);
        }
    }

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

void WorldObject::applyGlobalForce( Force *f) {

	list<MassPoint3D*>::iterator it, end = pointList.end();
	for (it = pointList.begin(); it != end; it++) {
	    (*it)->addForce(f);
	}
}

void WorldObject::pushObject (WorldObject *obj) {
    containsObject = true;
    objects.push_front(obj);
}

void WorldObject::pushSpring(MassPoint3D* start, MassPoint3D* end, float hardness){
	// Create the connection Springs:
	Spring* spring1 = new Spring(start, end, hardness);

	// Add Springs to the springList
	springList.push_front(spring1);
}


