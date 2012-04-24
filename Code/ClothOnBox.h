#pragma once

#include "WorldObject.h"
#include "Cloth.h"
#include "Cube.h"
#include "Vertex3D.h"

// for fun, let's drop a cloth on a nail
// cloth bending is required for the nail simulation
#if CLOTH_BENDING
#define CLOTHONBOX_NAIL 0
#endif

class ClothOnBox: public WorldObject {

	Cloth *cloth;
	Cube *box; // if we don't apply any force, it won't move
    Force ***forces; // force from the box on the cloth

#if CLOTH_BENDING == 1
    float basex, basey, basez, size;
#endif
#if CLOTHONBOX_NAIL == 1
    MassPoint3D *nail;
    int nail_i, nail_j; // pos of the nail in the mesh
    Spring* nailSprings[4];// on its 4 neighbours
#endif

public:
	
	ClothOnBox();
	ClothOnBox(float x, float y, float z);
	void draw();
	void timeStep(float);

};
