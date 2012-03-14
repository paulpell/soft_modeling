
#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "Vertex3D.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>

class Keyboard {
public:
	Keyboard();
	virtual ~Keyboard();


	void processInput(unsigned char key, int x, int y);

};

#endif /* KEYBOARD_H_ */
