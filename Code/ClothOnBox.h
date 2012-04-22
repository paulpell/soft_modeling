#pragma once

#include "WorldObject.h"
#include "Cloth.h"
#include "Cube.h"
#include "Vertex3D.h"

class ClothOnBox: public WorldObject {

	Cloth *cloth;
	Cube *box; // if we don't apply any force, it won't move
    Force ***forces; // force from the box on the cloth

    // for fun, let's drop a cloth on a nail
    Vertex3D *nail;

public:
	
	ClothOnBox();
	ClothOnBox(float x, float y, float z);
	void draw();
	void timeStep(float);

};
