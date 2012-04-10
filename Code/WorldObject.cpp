#include "WorldObject.h"

WorldObject::WorldObject(){
}

WorldObject::~WorldObject() {
	// TODO
}

void WorldObject::timeStep(float time){
}

void WorldObject::draw(){
	list<Spring*>::iterator it, listend = springList.end();
	for (it = springList.begin(); it != listend; it++) {
		(*it)->draw();
	}
}



