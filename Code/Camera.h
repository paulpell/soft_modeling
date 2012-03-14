#ifndef CAMERA_H_
#define CAMERA_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Keyboard.h"

class Camera {
public:
	Camera();
	Camera(Keyboard k);
	virtual ~Camera();

	void move_camera();

	//void movement(char key);


//private:

	Keyboard keyboard;

	void rotate_xaxis(int phi);
	void rotate_yaxis(int phi);
	void rotate_zaxis(int phi);

	void move_toX(int x);
	void move_toY(int y);
	void move_toZ(int z);

	int XCoord, YCoord, ZCoord;
	int currentXangle, currentYangle ,currentZangle;

};


#endif /* CAMERA_H_ */
