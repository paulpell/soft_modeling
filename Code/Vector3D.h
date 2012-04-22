// 14.03.2012

#ifndef VECTOR3D_H_
#define VECTOR3D_H_

class Vector3D {
public:
	// Con- and Destructor:
	Vector3D();
	Vector3D(float x, float y, float z);
	virtual ~Vector3D();

	// Properties:
	float x, y, z;
};

#endif /* VECTOR3D_H_ */
