
#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(Keyboard k)
{
	keyboard = k;
}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}

void Camera::rotate_xaxis(int phi){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(phi, 1, 0, 0);
}
void Camera::rotate_yaxis(int phi){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(phi, 0, 1, 0);
}
void Camera::rotate_zaxis(int phi){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(phi, 0, 0, 1);
}

void Camera::move_camera(){
	glLoadIdentity();
	move_toX(XCoord);
	move_toY(YCoord);
	move_toZ(ZCoord);
	//rotate_xaxis(currentXangle);
	//rotate_yaxis(currentYangle);
	//rotate_zaxis(currentZangle);
}

void Camera::move_toX(int x)
{
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(-x,0,0);
}

void Camera::move_toY(int y)
{
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0,-y,0);
}

void Camera::move_toZ(int z)
{
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0,0,-z);
}
