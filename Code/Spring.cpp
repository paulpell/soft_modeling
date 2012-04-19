
#include "Spring.h"

Spring::Spring(){
}

Spring::~Spring(){
}

// Creates a simple String
Spring::Spring(MassPoint3D *start, MassPoint3D *end, float elasticity) {
	this->start = start;
	this->end = end;


	this->elasticity = elasticity;
	this->org_length = getLength();

	intForceS = new Force(0, 0, 0);
	intForceE = new Force(0, 0, 0);
	start->addForce(intForceS);
	end->addForce(intForceE);
}


//void Spring::addMassPoint(MassPoint3D p){
//}

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
	cout << "spring length " << l << endl;
	float dx = (start->x - end->x) / l;
	float dy = (start->y - end->y) / l;
	float dz = (start->z - end->z) / l;
	float forceIntensity = - elasticity * (l - org_length);

	// if both are not anchors, divide the force by 2
	if (!(start->isAnchor || end->isAnchor)) forceIntensity /= 2.0f;

	cout << "spring update internal force to: " << dx*forceIntensity << "," << dy*forceIntensity << "," << dz*forceIntensity << endl;

	intForceS->x = dx * forceIntensity;
	intForceS->y = dy * forceIntensity;
	intForceS->z = dz * forceIntensity;

	// opposite force for both masspoints
	intForceE->x = -intForceS->x;
	intForceE->y = -intForceS->y;
	intForceE->z = -intForceS->z;

}

void Spring::setSize(int s){
	size = s;
}

void Spring::draw(){
	std::cout << "spring draw?? whos calling that" << std::endl;
	glLineWidth(size);
	glColor3f(1.0, 1.0, 1.0);
	float mcolor[] = { 1, 1, 1, 1.0f };
	//cout << "Sprign: " << start->y << endl;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
	glBegin(GL_LINES);
		start->draw();
		end->draw();
	glEnd();
}
