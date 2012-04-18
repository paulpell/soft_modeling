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

	// properties of rope:
	segments = 8; // must be at least 1 !!
	segsize = 3; // seglength without stress
	radius = 0.5;
	hardness = radius*10; // je dicker das seil desto fester :D

	// adding points to ropes 'spline'
	int x = start->x;
	int z = start->z;
	int y = start->y;
	for(int i=1; i<=segments; i++){
		MassPoint3D* spline = new MassPoint3D(x+i*segsize, y, z);
		cout << "adding" << x+i*segsize << " "  << y << " " << z << endl;
		addNode(spline);
	}
cout << "Rope completed with " << pointList.size() << " points" << endl;

    if (Rope::ropetextureName == 0)
        glGenTextures(1, &Rope::ropetextureName);
}

Texture Rope::ropetexture = Texture("rope.512x512.bmp", 512, 512);
GLuint Rope::ropetextureName = 0;


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
	float den = veclength(x1, y1, z1) * veclength(x2, y2, z2);
	float nom = vecdot(x1,y1,z1,  x2,y2,z2);
	float val = nom / den;
	return acos(val) * 180.0 / 3.14159265;
}

void Rope::draw(){

	// some init:
	list<Spring*>::iterator seghead, listend;	
	float angle = 0;
	float seglen = segsize;
	
	// now loop over segments:
	seghead = springList.begin();
	listend = springList.end();

   // generate, bind the texture
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, ropetextureName);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   int width = ropetexture.imagewidth;
   int height = ropetexture.imageheight;
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ropetexture.xData);

	while ( seghead != listend ) {

		// get the coords of segment:
		float x1 = (*seghead)->end->x;
		float y1 = (*seghead)->end->y;
		float z1 = (*seghead)->end->z;

		float x2 = (*seghead)->start->x;
		float y2 = (*seghead)->start->y;
		float z2 = (*seghead)->start->z;

		seghead++;

		// Draw MassPoints of the Rope: [only for debugging...]
		/*glPointSize(4);
		glColor3f(1,0,0);
		glBegin(GL_POINT);
			glVertex3f(x1, y1, z1+radius+0.1);// slightly dislocated so it is visible
			//cout << "pointH" << x1 << " "  << y1 << " " << z1 << endl;
		glEnd();
		glColor3f(0,0,1);
		glBegin(GL_POINT);
			glVertex3f(x2, y2, z2-radius-0.1); // slightly dislocated so it is visible
			//cout << "pointT" << x2 << " " << y2 << " " << z2 << endl;
		glEnd();*/		

		// compute length of current segment:
		seglen = veclength(x1-x2, y1-y2, z1-z2);
		// compute angle with segment and x axis:
		angle = vecangle(x2-x1, y2-y1, z2-z1, 1, 0, 0);

		// paint the tube of this segment
		//glColor3f(225.0/256.0,157.0/256.0,76.0/256.0); // some ropelike color...

 		glPushMatrix();
			glTranslatef(x1, y1, z1);
			glRotatef(90, 0, 1, 0); // because the rope hangs in x+ direction!
			glRotatef(-angle, 1, 0, 0); // because the rope will be bent around x
            GLUquadric* quad = gluNewQuadric();
            gluQuadricTexture(quad, true);
            gluQuadricDrawStyle(quad, GLU_FILL);
			gluCylinder(quad, radius, radius, seglen, 8, 1);

			// APPLY TEXTURE HERE
			// Texture ropetexture("rope.bmp", 512, 512);
            //

		glPopMatrix();
        
	}
    glDisable(GL_TEXTURE_2D);

}




