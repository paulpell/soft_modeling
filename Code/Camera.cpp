
#include "Camera.h"

Camera::Camera()
{
}


Camera::Camera(float x, float y, float z){
	position = Vector3D(x, y, z);

	// TODO some hardcoded parameters...
	y_rotation = -90;
	z_rotation = 140;
	distance = 10;	
}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}


void Camera::turn_horizontal(float a){
	y_rotation += a;
}
void Camera::turn_vertical(float a){
	z_rotation += a;
	// limit to 'human behavior':
	if (z_rotation < 40){
		z_rotation = 40;
	}
	if (z_rotation > 200){
		z_rotation = 200;
	}
}

void Camera::move_left(){
	position.x--;
}
void Camera::move_right(){
	position.x++;
}
void Camera::move_up(){
	h.y++;
}
void Camera::move_down(){
	h.y--;
}
void Camera::move_forward(){
	position.z--;
	//Vector3D dir = getViewingDirection();
	//position.x += dir.x / distance;
	//position.y += dir.y / distance;
	//position.z += dir.z / distance;
}
void Camera::move_back(){
	position.z++;
}

// Returns the point in space the camera is looking at:
Vector3D Camera::getViewingDirection(){
	// horizontal tilt:
	float PI = 3.14156;
	float degree = y_rotation * 2 * PI / 360;
	float x = cos(degree);
	float z = sin(degree);

	// vertical tilt:	
	degree = z_rotation * 2 * PI / 360;
	float y = cos(degree) + sin(degree);

	// 
	Vector3D v(x*distance, y*distance, z*distance);

	// TODO replace this with Vector Operator overloading...
	Vector3D q(position.x+v.x, position.y+h.y+v.y, position.z+v.z);

	//cout << "y Angle: " << y_rotation << endl;
	//cout << "z Angle: " << z_rotation << endl;
	//cout << "Looking at " << q.x << " " << q.y << " " << q.z << endl;
	return q;
}

Vector3D Camera::getPosition(){
	return position;
}
