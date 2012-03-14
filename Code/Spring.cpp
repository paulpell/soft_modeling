
#include "Spring.h"

// Creates a simple String
Spring::Spring(MassPoint3D *start, MassPoint3D *end, float strength) {
	this->start = start;
	this->end = end;
	this->strength = strength;
	this->org_length = getLength();
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

	end->timeStep(time);

}

void Spring::setSize(int s){
	size = s;
}

//#include <iostream>

void Spring::draw(){
	//std::cout << "spring draw" << std::endl;
	glLineWidth(size);
	glBegin(GL_LINES);
		start->draw();
		end->draw();
	glEnd();
}

