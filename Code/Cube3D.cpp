
#include "Cube3D.h"

Cube3D::Cube3D(){}

Cube3D::Cube3D(Vertex3D cp, int r)
{

	center = cp;
	radius = r;

	float x = center.getX();
	float y = center.getY();
	float z = center.getZ();

/*
	  E ---- F
	 /|     /|
	A ---- B |
	| | x  | |
	| H ---- G
	|/     |/
	D ---- C

	x = center
*/

	// Vertices:
	Vertex3D a = Vertex3D( x-radius, y+radius, z-radius);
	Vertex3D b = Vertex3D( x+radius, y+radius, z-radius);
	Vertex3D c = Vertex3D( x+radius, y-radius, z-radius);
	Vertex3D d = Vertex3D( x-radius, y-radius, z-radius);

	Vertex3D e = Vertex3D( x-radius, y+radius, z+radius);
	Vertex3D f = Vertex3D( x+radius, y+radius, z+radius);
	Vertex3D g = Vertex3D( x+radius, y-radius, z+radius);
	Vertex3D h = Vertex3D( x-radius, y-radius, z+radius);

	// Face1:
	face1 = Surface3D( a, b, c, d);
	face1.setColor(1, 0, 0);

	// Face2:
	face2 = Surface3D( h, g, f, e);
	face2.setColor(0.5, 0, 0);

	// Face3:
	face3 = Surface3D( e, f, b, a);
	face3.setColor(0, 1, 0);

	// Face4:
	face4 = Surface3D( d, c, g, h);
	face4.setColor(0, 0.5, 0);

	// Face5:
	face5 = Surface3D( b, f, g, c);
	face5.setColor(0, 0, 1);

	// Face6:
	face6 = Surface3D( a, e, h, d);
	face6.setColor(0, 0, 0.5);
}

Cube3D::~Cube3D()
{
}

void Cube3D::addTexture(Texture texture, int face){

	face4.setTexture(texture);
}

void Cube3D::draw()
{
	face1.draw();
	face2.draw();

	face3.draw();
	face4.draw();

	face5.draw();
	face6.draw();
}
