#include "Rope.h"

#include <iostream>
using namespace std;

Rope::Rope(){
}

/*
	This constructor creates a Rope.
	It is not a very dynamic editable Rope,
	but for now we need only a proof of concept!
*/
Rope::Rope(MassPoint3D* start){

	// our Rope has a fixed number of segments, with a fixed length
	// also our Rope initially will always hang horizontally into x+ direction
	// only the startpoint is given as parameter (for now)
	// the only anchor for now is the START

	// add the startpoint to the pointList
	pointList.push_front(start);
	start->setAnchor(true);

	// create and add other points to the pointList
	int x = start->x;
	int z = start->z;
	int y = start->y;	

	segments = 10;
	segsize = 1;
	hardness = 3;
	radius = 0.5;
	MassPoint3D* spline;
	for(int i=1; i<segments; i++){
		spline = new MassPoint3D(x+i*segsize, y, z);
		//cout << x << y << z << endl;
		//pointList.push_front( spline );
		addNode(spline);
	}

}

// this was used back when we added points manually....
void Rope::addNode(MassPoint3D* next){
	MassPoint3D* last = pointList.front();
	pointList.push_front(next);

	// Create the connection Springs:
	Spring* spring1 = new Spring(last, next, hardness);
	spring1->setSize(1); // remove later

	// Add Springs to the springList
	springList.push_front(spring1);
}


float Rope::veclength(float x1, float y1, float z1){
	return sqrt(x1*x1 + y1*y1 + z1*z1);
}

float Rope::vecdot(float x1, float y1, float z1, float x2, float y2, float z2){	
	return x1*x2+y1*y2+z1*z2;
}

float Rope::vecangle(float x1, float y1, float z1, float x2, float y2, float z2){
	float val = vecdot(x1, y1,z1, x2, y2, z2) / (veclength(x1, y1, z1) * veclength(x2, y2, z2)) ;
	return cos(val);
}

void Rope::draw(){
	list<Spring*>::iterator it, listend = springList.end();
	int angle = 90;
	int seglen = segsize;

	// loop over segments
	for (it = springList.begin(); it != listend; it++) {
		
		// fetch 1st and following wirbel
		int x1 = (*it)->start->x;
		int y1 = (*it)->start->y;
		int z1 = (*it)->start->z;

		list<Spring*>::iterator it2 = it++;
		int x2 = (*it2)->start->x;
		int y2 = (*it2)->start->y;
		int z2 = (*it2)->start->z;

		// compute angle and length here:
		seglen = veclength(x1-x2, y1-y2, z1-z2);
		angle = vecangle(x1, y1, z1, x2, y2, z2);

		// paint the tube of this segment
		glColor3f(0,0,0);
		glPushMatrix();
			glTranslatef(x1, y1, z1);
			glRotatef(angle, 1, 0, 0);
			gluCylinder(gluNewQuadric(), 1, 1, seglen, 8, 1);
			//glutSolidCube(1);
		glPopMatrix();

	}
}




