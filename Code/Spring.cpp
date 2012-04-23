
#include "Spring.h"

Spring::Spring(){
}

Spring::~Spring(){
}

// Creates a simple String
Spring::Spring(MassPoint3D *start, MassPoint3D *end, float hardness) {
	this->start = start;
	this->end = end;


	this->org_length = getLength();
	this->hardness = hardness / org_length;

	intForceS = new Force(0, 0, 0);
	intForceE = new Force(0, 0, 0);
	start->addForce(intForceS);
	end->addForce(intForceE);
}

float Spring::getLength(){
	float x = start->x - end->x;
	float y = start->y - end->y;
	float z = start->z - end->z;

	float sum =  x*x + y*y + z*z;
	return sqrt(sum);
}

void Spring::timeStep(float time){

	// update the force
	float l = getLength();
	float dx = (start->x - end->x) / l;
	float dy = (start->y - end->y) / l;
	float dz = (start->z - end->z) / l;
	float forceIntensity = - hardness * (l - org_length);

    

	// if both are not anchors, divide the force by 2
	if (!(start->isAnchor || end->isAnchor)){
		forceIntensity /= 2.0f;
	}

	intForceS->x = dx * forceIntensity;
	intForceS->y = dy * forceIntensity;
	intForceS->z = dz * forceIntensity;

	// opposite force for both masspoints
	intForceE->x = -intForceS->x;
	intForceE->y = -intForceS->y;
	intForceE->z = -intForceS->z;

}

void Spring::draw(){
	glLineWidth(1);
	glColor3f(1.0, 1.0, 1.0);
	float mcolor[] = { 1, 1, 1, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);

	glBegin(GL_LINES);
		start->draw();
		end->draw();
	glEnd();
}
