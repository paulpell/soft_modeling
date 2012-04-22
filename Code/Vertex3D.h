// 29.feb.2012

#ifndef VERTEX3D_H_
#define VERTEX3D_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdlib.h>

#include "Vector3D.h"

class Vertex3D {//: public Vector3D {
public:
	// Con- und Destructor:
	Vertex3D();
	Vertex3D(float x, float y, float z);
	virtual ~Vertex3D();

	// Graphical represenation:
	void draw();
	void setSize(int s);

	// Getter and Setter:
	float getX();
	float getY();
	float getZ();


	float x, y, z;
	int size;
};

#endif /* VERTEX3D_H_ */
