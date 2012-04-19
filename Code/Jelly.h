//#pragma once

#include "MassPoint3D.h"
#include "WorldObject.h"
#include "Texture.h"
#include <GL/gl.h>

class Jelly : public WorldObject {

    static Texture texture;
    static GLuint textureName;

    int segments;
    float basex, basey, basez; // position of one corner
    float size;
    float normx(float);
    float normy(float);
    float normz(float);

    MassPoint3D**** mesh;
    MassPoint3D*** faces;
public:
    
    Jelly();
    ~Jelly();

    void draw();

};
