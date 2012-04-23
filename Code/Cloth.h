// 18.04.2012

#ifndef CLOTH_H_
#define CLOTH_H_

#include "WorldObject.h"
#include "MassPoint3D.h"
#include "Spring.h"

#include "Texture.h"

#include <list>

class Cloth : public WorldObject {
	//static GLuint textureName;
	//static Texture texture;

public:
	// Con- and Destructor:
	Cloth();
	Cloth(MassPoint3D* start, int segments, float size);
	Cloth(MassPoint3D* start, int segments, float size, float totalMass);

	void reset();
    void anchor3();
    void anchorSide();
	void draw();

    int getSegments() { return segments; }
    MassPoint3D* mesh(int i, int j);

	// sry nicht nice
	float veclength(float x1, float y1, float z1);
	float vecdot(float x1, float y1, float z1, float x2, float y2, float z2);
	float vecangle(float x1, float y1, float z1, float x2, float y2, float z2);

private:
    // for multiple constructors
    void init(MassPoint3D* start, int segments, float size, float singleMass);

	void addSprings();

	MassPoint3D*** meshNet; //2d array of pointers

	// 
	int segments;
	float hardness, segsize;
    float totalMass;

	GLuint textureName;
	//Texture ropetexture;
};

#endif
