#ifndef WORLD3D_H_
#define WORLD3D_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Cube3D.h"

#include "Camera.h"
#include "Vertex3D.h"
#include "Surface3D.h"
#include "Texture.h"

#include "Keyboard.h"

class World3D {
public:
	World3D(Camera c);
	virtual ~World3D();






	void init(int argc, char** argv);


	void createObjects();

	void reshape(int w, int h);


	void display(void);

	void run();
	
	Camera camera;
	Cube3D aCube;

};


#endif /* WORLD3D_H_ */
