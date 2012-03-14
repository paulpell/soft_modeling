
#ifndef CUBE3D_H_
#define CUBE3D_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdlib.h>
#include <stdio.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Surface3D.h"
#include "Vertex3D.h"


class Cube3D {
public:
	Cube3D();
	Cube3D(Vertex3D c, int r);
	virtual ~Cube3D();

	void draw();

	void addTexture(Texture texture, int face);

private:

	int radius;
	Vertex3D center;
	
	Surface3D face1, face2, face3, face4, face5, face6;
};

#endif /* CUBE3D_H_ */
