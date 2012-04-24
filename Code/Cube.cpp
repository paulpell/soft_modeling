
#include "Cube.h"


Cube::Cube() {
    basex = basey = basez = 0;
    size = 5;
    init();
}

Cube::Cube(float x, float y, float z) {
    basex = x;
    basey = y;
    basez = z;
    size = 5;
    init();
}

Cube::Cube(float x, float y, float z, float size) {
    basex = x;
    basey = y;
    basez = z;
    this->size = size;
    init();
}


void Cube::init() {
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

Texture Cube::texture = Texture("jelly.512x512.bmp", 512, 512);
GLuint Cube::textureName = 0;

Cube::~Cube(){}

// *** Some collision detection helpers
// Assume position aligned with the axes =)
bool Cube::isPointInBaseSquare(MassPoint3D *p) {
    MassPoint3D *m = mesh[0][0][0], *m2 = mesh[segments][segments][segments];
    return (p->x >= m->x) && (p->x <= m2->x) && (p->z >= m->z) && (p->z <= m2->z);
}
bool Cube::isPointInside(MassPoint3D *p) {
    float x = p->x, y = p->y, z = p->z;
	MassPoint3D *m1 = mesh[0][0][0], *m2 = mesh[segments][segments][segments];
	return x >m1->x && x < m2->x 
        && y > m1->y && y < m2->y
        && z > m1->z && z < m2->z;

}
bool Cube::isPointInActiveArea(MassPoint3D *p) {
    float x = p->x, y = p->y, z = p->z;
	MassPoint3D *m1 = mesh[0][0][0], *m2 = mesh[segments][segments][segments];
	return x >m1->x - delta && x < m2->x + delta
        && y > m1->y -delta && y < m2->y + delta
        && z > m1->z -delta && z < m2->z + delta;

}
int Cube::relpos(MassPoint3D* m) {
    // bit  position
    // 0    < x
    // 1    > x + size
    // 2    < y
    // 3    > y + size
    // 4    < z
    // 5    > z + size
    int i = 0;
    if (m->x <= basex) i += 1;
    if (m->x >= basex+size) i += 2;
    if (m->y <= basey) i += 4;
    if (m->y >= basey+size) i += 8;
    if (m->z <= basez) i+= 16;
    if (m->z >= basez+size) i += 32;
    return i;
}
// The given point will be set at the border, and its velocity inverted
// The argument f is the force exerted by the box on p; it can be NULL
void Cube::collide(MassPoint3D *p, Force *f,float dt) {
    int s = segments;
    float xMean = (mesh[s][0][0]->x - mesh[0][0][0]->x) / 2.;
    float yMean = (mesh[0][s][0]->y - mesh[0][0][0]->y) / 2.;
    float zMean = (mesh[0][0][s]->z - mesh[0][0][0]->z) / 2.;
    int xLow = (p->x <= xMean) ? 1 : 0;
    int yLow = (p->y <= yMean) ? 1 : 0;
    int zLow = (p->z <= zMean) ? 1 : 0;
    // for each axis, nearer of which plane?
    float xDist = xLow ? p->x - mesh[0][0][0]->x : mesh[s][0][0]->x - p->x;
    float yDist = yLow ? p->y - mesh[0][0][0]->y : mesh[s][0][0]->y - p->y;
    float zDist = zLow ? p->z - mesh[0][0][0]->z : mesh[s][0][0]->z - p->z;
    
    // find to which side the point p is the nearest, and update p wrt that side
    int minAxis = 0; // by default, p is nearest of an x aligned side
    float delta = 0.5;
    if (yDist < xDist) minAxis = 1;
    if (zDist < xDist && zDist < yDist) minAxis = 2;

    // replace the velocity by a constant and cancel the forces wrt to the minAxis
    float bounce_velocity = .1;
    // the formula below is used to adjust the force by the box
#define forceTrans(c) if (f) f->c -= .8 * p->totalForce.c

    // check all three axis to see whether it's happening in that direction
    if (minAxis == 0) {
        forceTrans(x);
        if (xLow) {
            p->velocity.x = -bounce_velocity;
            p->x = mesh[0][0][0]->x - delta;
        }
        else {
            p->velocity.x = bounce_velocity;
            p->x = mesh[s][0][0]->x + delta;
        }
    }
    if (minAxis == 1) {
        forceTrans(y);
        if (yLow) {
            p->velocity.y = -bounce_velocity;
            p->y = mesh[0][0][0]->y - delta;
        }
        else {
            p->velocity.y = bounce_velocity;
            p->y = mesh[0][s][0]->y + delta;
        }
    }
    if (minAxis == 2) {
        forceTrans(z);
        if (zLow) {
            p->velocity.z = -bounce_velocity;
            p->z = mesh[0][0][0]->z - delta;
        }
        else {
            p->velocity.z = bounce_velocity;
            p->z = mesh[0][0][s]->z + delta;
        }
    }
}

inline float Cube::normx(float f) { return (f - basex) / size;}
inline float Cube::normy(float f) { return (f - basey) / size;}
inline float Cube::normz(float f) { return (f - basez) / size;}

float Cube::maxY() {
    return size + basey;
}

void Cube::timeStep(float dt) {
    WorldObject::timeStep(dt);
    if (melting) {
	    list<Spring*>::iterator it, end = springList.end();
	    for (it = springList.begin(); it != end; it++) {
	   // 	(*it)->org_length *= .99;
	    	(*it)->hardness *= .90;
	    }
    }
}

void Cube::setMelting() {
    melting = true;
}

void Cube::draw(){

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


