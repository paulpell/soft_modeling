
#ifndef SURFACE3D_H_
#define SURFACE3D_H_

#include "Vertex3D.h"
#include "Texture.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>

class Surface3D {
public:
	Surface3D();
	Surface3D(Vertex3D a, Vertex3D b, Vertex3D c, Vertex3D d);
	virtual ~Surface3D();

	// Setter functions:
	void setColor(float r, float g, float b);

	void setTexture(Texture texture);
	void deleteTexture();

	void draw();

private:
	Vertex3D topright, topleft, bottomleft, bottomright;
	float colorR, colorG, colorB;

	bool hasTexture;
	GLuint textureName;

};

#endif /* SURFACE3D_H_ */
