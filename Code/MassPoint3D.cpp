
#include "MassPoint3D.h"


using namespace std;


MassPoint3D::MassPoint3D() : Vertex3D(0, 0, 0){
}

MassPoint3D::MassPoint3D(float x, float y, float z) : Vertex3D(x, y, z) {
	//forces = 1;
	//forceList = new Force;

	velocity = Vector3D(0, 0, 0);
}

MassPoint3D::~MassPoint3D(){
}

void MassPoint3D::addForce(Force *f){

}

void MassPoint3D::setAnchor(bool b){
	isAnchor = b;
}


void MassPoint3D::timeStep(float time){

	// compute overall force
	Force temp(0.0, 0.0, 0.0);
//	list<Force*>::iterator it, listend = forces.end();
//	for (it = forces.begin(); it != listend; it++) {
//		temp += *it;
//	}

	// update velocity
	//velocity += time* (temp->x / mass);

	// update Coords
	//x += time* velocity->x;
	//y
	//z

}
