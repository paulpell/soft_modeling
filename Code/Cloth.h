// 18.04.2012

#ifndef CLOTH_H_
#define CLOTH_H_

#include "WorldObject.h"
#include "MassPoint3D.h"
#include "Spring.h"

#include "Texture.h"

#include <list>

// enable adding of temporary points to bend the cloth
#define CLOTH_BENDING 1


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
#if CLOTH_BENDING
    //void setBendings(int i, int j, MassPoint3D**);
    void bendCorner(int i, int j, MassPoint3D*);
    void bendEdge(int i, int j, MassPoint3D*, MassPoint3D*);
    void clearBend(int i, int j);
#endif
	// sry nicht nice
	float veclength(float x1, float y1, float z1);
	float vecdot(float x1, float y1, float z1, float x2, float y2, float z2);
	float vecangle(float x1, float y1, float z1, float x2, float y2, float z2);

private:
    // for multiple constructors
    void init(MassPoint3D* start, int segments, float size, float singleMass);

	void addSprings();
	MassPoint3D*** meshNet; //2d array of pointers
#if CLOTH_BENDING == 1
    MassPoint3D**** bendingPoints; // 2d arr which can hold 0,1 or 2 MassPoints
    // bending points can be added under these conditions:
    // one tile is added one point if it has hit a corner
    // one tile is divided in two by collision with an edge
    // To simplify at draw time the coded value at [i][j] is:
    // 0 -- no point
    // 1 -- a single point
    // x -- with two points, see bendEdge() in the .cpp for the details
    int **bendingPointsCoding;
#endif

	// 
	int segments;
	float hardness, segsize;
    float totalMass;

    // we need only one for all the Cloths
    static Texture texture;
	static GLuint textureName;
};

#endif
