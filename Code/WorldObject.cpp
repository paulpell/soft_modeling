#include "WorldObject.h"

WorldObject::WorldObject(){
}

WorldObject::~WorldObject() {
	// TODO
}

void WorldObject::timeStep(float time){
    
    //list<Spring>::iterator it, end = springList.end();
    list<Spring*>::iterator it, end = springList.end();
    for (it = springList.begin(); it != end; it++) {
        //(*it).timeStep(time);
        (*it)->timeStep(time);
    }

    //list<MassPoint3D>::iterator it2, end2 = pointList.end();
    list<MassPoint3D*>::iterator it2, end2 = pointList.end();
    for (it2 = pointList.begin(); it2 != end2; it2++) {
        //(*it2).timeStep(time);
        (*it2)->timeStep(time);
    }

    //cout << "ASOIDJAW:"<<endl;
}

void WorldObject::draw(){
	list<Spring*>::iterator it, listend = springList.end();
	//list<Spring>::iterator it, listend = springList.end();
	for (it = springList.begin(); it != listend; it++) {
		//(*it).draw();
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

