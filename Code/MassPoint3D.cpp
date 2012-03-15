
#include "MassPoint3D.h"


using namespace std;

MassPoint3D::MassPoint3D(){
	x = 0;
	y = 0;
	z = 0;	
}

MassPoint3D::MassPoint3D(float x, float y, float z){

	this->x = x;
	this->y = y;
	this->z = z;

	//forces = 1;
	//forceList = new Force;
	//velocity = Vector3D(0, 0, 0);
}

MassPoint3D::~MassPoint3D(){
}

void MassPoint3D::addForce(Force *f){

	forcelist.push_front(f);

}

void MassPoint3D::setAnchor(bool b){
	isAnchor = b;
}


void MassPoint3D::timeStep(float time){

	// compute overall force
	Force temp(0.0, 0.0, 0.0);
	list<Force*>::iterator it, listend = forcelist.end();
	for (it = forcelist.begin(); it != listend; it++) {
		temp.x += (*it)->x;
		temp.y += (*it)->y;
		temp.z += (*it)->z;
	}

	// update velocity
	velocity.x += time * (temp.x / mass);
	velocity.y += time * (temp.y / mass);
	velocity.z += time * (temp.z / mass);

	// TODO maybe store old location here

	// update Coords
	x += time* velocity.x;
	y += time* velocity.y;
	z += time* velocity.z;

}
