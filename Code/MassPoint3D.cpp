
#include "MassPoint3D.h"

static float dampening = .95;

using namespace std;
//
MassPoint3D::MassPoint3D(){
	x = 0;
	y = 0;
	z = 0;
	mass = 1;
	std::cout << "WARNING in Masspoint constructor :P" << std::endl;
	//MassPoint3D(0, 0, 0);
}

MassPoint3D::MassPoint3D(float x, float y, float z){

	this->x = x;
	this->y = y;
	this->z = z;

	mass = 1;
	isAnchor = false;

	//forces = 1;
	//forceList = new Force;
	velocity = Vector3D(0, 0, 0);
}

MassPoint3D::~MassPoint3D(){
}

void MassPoint3D::addForce(Force *f){

	forcelist.push_front(f);

}

void MassPoint3D::setAnchor(bool b){
	isAnchor = b;
}

void MassPoint3D::draw(){
	Vertex3D::draw();
	//std::cout << "Masspoint Coords: " << x << " " << y << " " <<  z << std::endl;
}

void MassPoint3D::timeStep(float time){
    using namespace std;
    std::cout << "MP3D" << std::endl;

	if (isAnchor) return;

	//std::cout << "time in masspoint: " << time << std::endl;

	// compute overall force
	Force temp(0.0, 0.0, 0.0);
    cout << "computing force\n";
	list<Force*>::iterator it, listend = forcelist.end();
	for (it = forcelist.begin(); it != listend; it++) {
           cout << "  adding x=" << (*it)->x << ",y="<<(*it)->y << ",z="<<(*it)->z <<endl;
		temp.x += (*it)->x;
		temp.y += (*it)->y;
		temp.z += (*it)->z;
	}

	std::cout << "m" << mass << std::endl;
    std::cout << "Force: " << temp.x << "," << temp.y << "," << temp.z << std::endl;
	// update velocity
	velocity.x += time * (temp.x / mass);
	velocity.y += time * (temp.y / mass);
	velocity.z += time * (temp.z / mass);

    cout << "velocity" << velocity.x << "," << velocity.y<<"," << velocity.z << " time=" << time << endl;
	// TODO maybe store old location here

    
    
	// update Coords
	x += time* velocity.x;
	y += time* velocity.y;
	z += time* velocity.z;

    velocity.x *= dampening;
    velocity.y *= dampening;
    velocity.z *= dampening;

    cout << "pos: " << x<<"," <<y<< "," << z<< endl;
}
