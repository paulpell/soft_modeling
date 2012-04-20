#ifndef CAMERA_H_
#define CAMERA_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Keyboard.h"
#include "Vector3D.h"
#include <math.h>

//#include <iostream>
//using namespace std;

class Camera {
public:
	Camera();
	Camera(float x, float y, float z);
	virtual ~Camera();

	void move_camera();

	void move_left();
	void move_right();
	void move_up();
	void move_down();
	void move_forward();
	void move_back();

	void turn_vertical(float a);
	void turn_horizontal(float a);

	//int XCoord, YCoord, ZCoord;
	//int currentXangle, currentYangle ,currentZangle;

	Vector3D getViewingDirection();
	Vector3D getPosition();

private:

	Vector3D position, h;
	float z_rotation, y_rotation, distance;

};


#endif /* CAMERA_H_ */
