#pragma once

#include "MassPoint3D.h"
#include "WorldObject.h"
#include "Texture.h"
#include <GL/gl.h>

class Cube : public WorldObject {

    static Texture texture;
    static GLuint textureName;

    int segments;
    float hardness;
    float basex, basey, basez; // position of one corner
    float size;
    float normx(float);
    float normy(float);
    float normz(float);

    bool melting;

    void init();

public:

    MassPoint3D**** mesh;
    MassPoint3D*** faces;
    
    Cube();
    Cube(float x, float y, float z);
    Cube(float x, float y, float z, float size);
    ~Cube();

    void draw();
    void timeStep(float);
    void setMelting();
    
    float maxY();

    void collide(MassPoint3D*, Force*, float dt);
    bool isPointInBaseSquare (MassPoint3D*);
	bool isPointInside(MassPoint3D*);

};
