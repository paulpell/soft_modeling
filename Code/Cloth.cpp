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
	segments = 8; // must be at least 1 !!
	segsize = 2; // seglength without stress
	hardness = 4; // je dicker das seil desto fester :D

	// adding points to cloth mesh
	int x = start->x;
	int z = start->z;
	int y = start->y;

	meshNet = new MassPoint3D**[segments];
	for(int i=0; i<=segments; i++){
		meshNet[i] = new MassPoint3D*[segments];
		for(int j=0; j<=segments; j++){

			// create a new MassPoint for this object:
			MassPoint3D* spline = new MassPoint3D(x+i*segsize, y, z+j*segsize, 4);

			
			// put them in pointList for physics effects (superclass compatibility!)
			pointList.push_front(spline); 

			// also put them into a matrix (makes creating springs much easier!)
			meshNet[i][j] = spline;

		}
	}


	// compute internal Springs:
	addSprings();

	// prePare texture:
	Texture texture = Texture("cloth2.bmp", 512, 512);

	glGenTextures(1, &textureName);
	glBindTexture(GL_TEXTURE_2D, textureName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.imagewidth, texture.imageheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.xData );

}

void Cloth::anchor3() {
	// some hardcoded anchors
    meshNet[0][0]->setAnchor(true);
    meshNet[0][segments-1]->setAnchor(true);
    meshNet[segments-1][segments-1];
}

void Cloth::anchorSide() {
    for (int i=0; i<segments+1; i++) {
        for (int j=0; j<segments+1; j++) {
            if (i == 0) meshNet[i][j]->setAnchor(true);
            else meshNet[i][j]->setAnchor(false);
        }
    }
}
    


// this was used back when we added points manually....
void Cloth::addSprings(){

	MassPoint3D *s, *e;

	// add horizontal springs
	for(int i=0; i<=segments; i++){
		for(int j=0; j<segments; j++){
			s = meshNet[i][j];
			e = meshNet[i][j+1];
			pushSpring(s, e, hardness);
		}
	}

	// add vertical springs
	for(int i=0; i<=segments; i++){
		for(int j=0; j<segments; j++){
			s = meshNet[j][i];
			e = meshNet[j+1][i];
			pushSpring(s, e, hardness);
		}
	}

	// add diagonal springs (optional)
	for(int i=0; i<segments; i++){
		for(int j=0; j<segments; j++){
			s = meshNet[i][j];
			e = meshNet[i+1][j+1];
			pushSpring(s, e, hardness);
		}
	}

	//cout << "coth with springs: " << springList.size() << endl;	
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

	#define SPRING_FRAME 0

	#if SPRING_FRAME
		WorldObject::draw();
	#else

	for(int i=0; i<segments; i++){
	for(int j=0; j<segments; j++){

		MassPoint3D* a = meshNet[i][j];
		MassPoint3D* b = meshNet[i][j+1];
		MassPoint3D* c = meshNet[i+1][j];
		MassPoint3D* d = meshNet[i+1][j+1];
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureName);

		glColor3f(1,1,1);
		float mcolor[] = { 1, 1, 1, 1.0f };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);	
		glBegin(GL_POLYGON);
			glTexCoord2d(0.0, 0.0);
			glVertex3f(a->x, a->y, a->z);
			glTexCoord2d(1.0, 0.0);
			glVertex3f(b->x, b->y, b->z);
			glTexCoord2d(0.0, 1.0);
			glVertex3f(c->x, c->y, c->z);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.0, 0.0);
			glVertex3f(d->x, d->y, d->z);
			glTexCoord2d(1.0, 0.0);
			glVertex3f(b->x, b->y, b->z);
			glTexCoord2d(0.0, 1.0);
			glVertex3f(c->x, c->y, c->z);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	}

	#endif

}
