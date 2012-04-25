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
    void bendEdge(int i, int j, MassPoint3D*, MassPoint3D*, int[2]);
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
    // bending points can be added under these conditions:
    // if a tile hits a corner, there might be upto 3 bending points: the corner itself,
    // and the two other end points for the edges
    // Or one tile is divided in two by collision with an edge.
    // To simplify at draw time the coded value at [i][j] is:
    // the first bit is 0 or 1, depending if there is a corner 
    // the remaining bits are encoded according to Cloth.h
    int **bendingPointsCoding;
    MassPoint3D*** middleBendingPoints; // 2d arr of pointers, to either 0 or 1 elmt
    MassPoint3D**** edgeBendingPoints; // 2d arr which can hold 0,1 or 2 MassPoints
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
