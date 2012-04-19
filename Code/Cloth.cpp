#include "Cloth.h"

#include <iostream>
using namespace std;

Cloth::Cloth(){
}

/*
	This constructor creates a Cloth.
	It is not a very dynamic editable Cloth,
	but for now we need only a proof of concept!
*/
Cloth::Cloth(MassPoint3D* start){

	// our Cloth has a fixed number of mesh size and mesh amount

	// also our Cloth initially will always hang horizontally into x+ z- direction
	// only the startpoint is given as parameter (for now)

	// anchor is picked manually

	// properties of rope:
	segments = 5; // must be at least 1 !!
	segsize = 4; // seglength without stress
	radius = 0.5;
	hardness = radius*10; // je dicker das seil desto fester :D

	// adding points to cloth mesh
	int x = start->x;
	int z = start->z;
	int y = start->y;

	meshNet = new MassPoint3D**[segments];
	for(int i=1; i<=segments; i++){
		meshNet[i-1] = new MassPoint3D*[segments];
		for(int j=1; j<=segments; j++){

			// create a new MassPoint for this object:
			MassPoint3D* spline = new MassPoint3D(x+i*segsize, y, z+j*segsize);


			// some hardcoded anchors
			if(i==1 && j==1){
				spline->setAnchor(true);
			}
			if(i==4 && j==4){
				//spline->setAnchor(true);
			}
			
			// debugg info:
			//cout << "cloth adding" << x+i*segsize << " "  << y << " " << z+j*segsize << endl;

			// put them in pointList for physics effects (superclass compatibility!)
			pointList.push_front(spline); 

			// also put them into a matrix (makes creating springs much easier!)
			meshNet[i-1][j-1] = spline;

		}
	}

	// compute internal Springs:
	addSprings();


	cout << "Cloth completed with " << pointList.size() << " points" << endl;

	if (Cloth::ropetextureName == 0)
		glGenTextures(1, &Cloth::ropetextureName);
}

Texture Cloth::ropetexture = Texture("cloth.bmp", 512, 512);
GLuint Cloth::ropetextureName = 0;



// this was used back when we added points manually....
void Cloth::addSprings(){
	//cout << "adding springs now" << endl;

	MassPoint3D *s, *e;

	// add horizontal springs
	for(int i=1; i<=segments; i++){
		for(int j=2; j<=segments; j++){
			s = meshNet[i-1][j-2];
			e = meshNet[i-1][j-1];
			pushSpring(s, e, hardness);
		}
	}

	// add vertical springs
	for(int i=2; i<=segments; i++){
		for(int j=1; j<=segments; j++){
			s = meshNet[i-1][j-1];
			e = meshNet[i-2][j-1];
			pushSpring(s, e, hardness);
		}
	}	

	// add diagonal springs
	for(int i=2; i<=segments; i++){
		for(int j=2; j<=segments; j++){
			s = meshNet[i-1][j-2];
			e = meshNet[i-2][j-1];
			pushSpring(s, e, hardness);
		}
	}	
}



float Cloth::veclength(float x1, float y1, float z1){
	return sqrt(x1*x1 + y1*y1 + z1*z1);
}

float Cloth::vecdot(float x1, float y1, float z1, float x2, float y2, float z2){	
	return x1*x2+y1*y2+z1*z2;
}

float Cloth::vecangle(float x1, float y1, float z1, float x2, float y2, float z2){
	float den = veclength(x1, y1, z1) * veclength(x2, y2, z2);
	float nom = vecdot(x1,y1,z1,  x2,y2,z2);
	float val = nom / den;
	return acos(val) * 180.0 / 3.14159265;
}

void Cloth::draw(){

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

		// Draw MassPoints + Wireframe of the Cloth: [only for debugging...]
		glPointSize(4);
		glColor3f(1,0,0);
		glBegin(GL_POINT);
			glVertex3f(x1, y1, z1+0.1);// slightly dislocated so it is visible
			//cout << "pointH" << x1 << " "  << y1 << " " << z1 << endl;
		glEnd();
		glColor3f(0,0,1);
		glBegin(GL_POINT);
			glVertex3f(x2, y2, z2-0.1); // slightly dislocated so it is visible
			//cout << "pointT" << x2 << " " << y2 << " " << z2 << endl;
		glEnd();
		glLineWidth(1);
		glColor3f(.7,.7,.7);
		glBegin(GL_LINE);
			glVertex3f(x1, y1, z1);			
			glVertex3f(x2, y2, z2);
		glEnd();	

		// compute length of current segment:
		//seglen = veclength(x1-x2, y1-y2, z1-z2);
		// compute angle with segment and x axis:
		//angle = vecangle(x2-x1, y2-y1, z2-z1, 1, 0, 0);

		// paint the tube of this segment
		//glColor3f(225.0/256.0,157.0/256.0,76.0/256.0); // some ropelike color...

 		/*glPushMatrix();
			glTranslatef(x1, y1, z1);
			glRotatef(90, 0, 1, 0); // because the rope hangs in x+ direction!
			glRotatef(-angle, 1, 0, 0); // because the rope will be bent around x
			GLUquadric* quad = gluNewQuadric();
			gluQuadricTexture(quad, true);
			gluQuadricDrawStyle(quad, GLU_FILL);
			gluCylinder(quad, radius, radius, seglen, 8, 1);

			// APPLY TEXTURE HERE
			// Texture ropetexture("rope.bmp", 512, 512);
		glPopMatrix();*/
        
	}
    glDisable(GL_TEXTURE_2D);

}




