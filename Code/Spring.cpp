
#include "Spring.h"

// Creates a simple String
Spring::Spring(MassPoint3D *start, MassPoint3D *end, float strength) {
	this->start = start;
	this->end = end;


	this->strength = strength;
	this->org_length = getLength();

	internalForce = new Force(0, 0, 0);
	start->addForce(internalForce);
	end->addForce(internalForce);
}

Spring::Spring(){
}

Spring::~Spring(){
}

//void Spring::addMassPoint(MassPoint3D p){
//}

float Spring::getLength(){
	float x = start->x - end->x;
	float y = start->y - end->y;
	float z = start->z - end->z;

	return sqrt( x*x + y*y + z*z);
}

void Spring::timeStep(float time){

	// update the force
	float l = getLength();
	float dx = (start->x - end->x) / l;
	float dy = (start->y - end->y) / l;
	float dz = (start->z - end->z) / l;
	float forceIntensity = - strength * (l - org_length);

	// if both are not anchors, divide the force by 2
	if (!(start->isAnchor || end->isAnchor)) forceIntensity /= 2;

	internalForce->x = dx * forceIntensity;
	internalForce->y = dy * forceIntensity;
	internalForce->z = dz * forceIntensity;


	//if(start- TODO dont move anchors
	start->timeStep(time);


	// opposite force for both masspoints
	internalForce->x *= -1;
	internalForce->z *= -1;
	internalForce->y *= -1;

	end->timeStep(time);

}

void Spring::setSize(int s){
	size = s;
}

//#include <iostream>

void Spring::draw(){
	//std::cout << "spring draw" << std::endl;
	glLineWidth(size);
	glColor3f(1.0, 1.0, 1.0);
	float mcolor[] = { 1, 1, 1, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
	glBegin(GL_LINES);
		start->draw();
		end->draw();
	glEnd();
}

