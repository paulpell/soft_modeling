
#include "Jelly.h"


Jelly::Jelly() {
    init();
}

Jelly::Jelly(float x, float y, float z) {
    basex = x;
    basey = y;
    basez = z;
    init();
}

void Jelly::init() {
    size = 5;
    segments = 1;
    hardness = 1;
    melting = false;

    //basex = -20, basey = 20, basez = 0;

    mesh = new MassPoint3D***[segments+1];
    faces = new MassPoint3D**[6];
    for (int i=0; i<segments+1; i++) {
        mesh[i] = new MassPoint3D**[segments+1];
        for (int j=0; j<segments+1;j++) {
            mesh[i][j] = new MassPoint3D*[segments+1];
            for (int k=0; k<segments+1;k++) {
                mesh[i][j][k] = new MassPoint3D(basex+i*size, basey+j*size, basez+k*size);
                pointList.push_front(mesh[i][j][k]);
            }
        }
    }
    mesh[0][0][0]->setAnchor(true);
    mesh[1][0][0]->setAnchor(true);
    mesh[0][0][1]->setAnchor(true);
    mesh[1][0][1]->setAnchor(true);

    for (int i=0; i<6; i++) faces[i] = new MassPoint3D*[4];

    faces[0][0] = mesh[0][0][0];
    faces[0][1] = mesh[0][0][1];
    faces[0][2] = mesh[0][1][1];
    faces[0][3] = mesh[0][1][0];

    faces[1][0] = mesh[1][0][0];
    faces[1][1] = mesh[1][0][1];
    faces[1][2] = mesh[1][1][1];
    faces[1][3] = mesh[1][1][0];

    faces[2][0] = mesh[0][0][0];
    faces[2][1] = mesh[0][0][1];
    faces[2][2] = mesh[1][0][1];
    faces[2][3] = mesh[1][0][0];

    faces[3][0] = mesh[0][1][0];
    faces[3][1] = mesh[0][1][1];
    faces[3][2] = mesh[1][1][1];
    faces[3][3] = mesh[1][1][0];

    faces[4][0] = mesh[0][0][0];
    faces[4][1] = mesh[0][1][0];
    faces[4][2] = mesh[1][1][0];
    faces[4][3] = mesh[1][0][0];

    faces[5][0] = mesh[0][0][1];
    faces[5][1] = mesh[0][1][1];
    faces[5][2] = mesh[1][1][1];
    faces[5][3] = mesh[1][0][1];

    
    // add springs
    // TODO find nice technique to add them
    /*
    for (int i=0; i<segments; i++) {
        for (int j=0; j<segments; j++) {
            for (int k=0; k<segments; k++) {
                springList.push_front(new Spring(mesh[i][j][k], mesh[
            }
        }
    }
    */
    springList.push_front(new Spring(mesh[0][0][0], mesh[0][0][1], hardness));
    springList.push_front(new Spring(mesh[0][0][0], mesh[0][1][0], hardness));
    springList.push_front(new Spring(mesh[0][0][0], mesh[1][0][0], hardness));

    springList.push_front(new Spring(mesh[0][0][1], mesh[1][0][1], hardness));
    springList.push_front(new Spring(mesh[0][0][1], mesh[0][1][1], hardness));

    springList.push_front(new Spring(mesh[0][1][0], mesh[1][1][0], hardness));
    springList.push_front(new Spring(mesh[0][1][0], mesh[0][1][1], hardness));

    springList.push_front(new Spring(mesh[1][0][0], mesh[1][1][0], hardness));
    springList.push_front(new Spring(mesh[1][0][0], mesh[1][0][1], hardness));

    springList.push_front(new Spring(mesh[1][1][1], mesh[1][1][0], hardness));
    springList.push_front(new Spring(mesh[1][1][1], mesh[0][1][1], hardness));
    springList.push_front(new Spring(mesh[1][1][1], mesh[1][0][1], hardness));

    // 4 diagonals
    springList.push_front(new Spring(mesh[1][1][1], mesh[0][0][0], hardness));
    springList.push_front(new Spring(mesh[0][1][1], mesh[1][0][0], hardness));
    springList.push_front(new Spring(mesh[1][0][1], mesh[0][1][0], hardness));
    springList.push_front(new Spring(mesh[1][1][0], mesh[0][0][1], hardness));

    // cross on the top
    springList.push_front(new Spring(mesh[1][1][0], mesh[0][1][1], hardness));
    springList.push_front(new Spring(mesh[0][1][0], mesh[1][1][1], hardness));

}

Texture Jelly::texture = Texture("jelly.512x512.bmp", 512, 512);
GLuint Jelly::textureName = 0;

Jelly::~Jelly(){}

inline float Jelly::normx(float f) { return (f - basex) / size;}
inline float Jelly::normy(float f) { return (f - basey) / size;}
inline float Jelly::normz(float f) { return (f - basez) / size;}

void Jelly::timeStep(float dt) {
    WorldObject::timeStep(dt);
    if (melting) {
	    list<Spring*>::iterator it, end = springList.end();
	    for (it = springList.begin(); it != end; it++) {
	   // 	(*it)->org_length *= .99;
	    	(*it)->hardness *= .90;
	    }
    }
}

void Jelly::setMelting() {
    melting = true;
}

void Jelly::draw(){

	#define SPRING_FRAME 0

	#if SPRING_FRAME
		WorldObject::draw();
	#else

    if (textureName == 0) {
        glGenTextures(1, &textureName);
        glBindTexture(GL_TEXTURE_2D, textureName);

	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.imagewidth, texture.imageheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.xData);
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureName);
    float mcolor[] = {1,1,1,1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);

    for (int face=0; face<6; face++) {
        glBegin(GL_QUADS);
        for (int i=0; i<4; i++) {
            MassPoint3D *mp1 =faces[face][i];
            glTexCoord2d(i%2, i/2); glVertex3f(mp1->x, mp1->y, mp1->z);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);


	#endif
}


