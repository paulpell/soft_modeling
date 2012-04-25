#include "Cloth.h"

#include <iostream>
using namespace std;

Cloth::Cloth(){
}
/*
	This constructor creates a Cloth.
	It is not a very dynamic editable Cloth,
	but for now we need only a proof of concept!

	// our Cloth has a fixed number of mesh size and mesh amount

	// also our Cloth initially will always hang horizontally into x+ z- direction

	// anchor is picked manually
*/
Cloth::Cloth(MassPoint3D* start, int segments, float size) {
    init(start, segments, size, 4);
}
Cloth::Cloth(MassPoint3D* start, int segments, float size, float singleMass){
    init(start, segments, size, singleMass);
}

// initialize static data
GLuint Cloth::textureName = 0;
Texture Cloth::texture(NULL, 0, 0);


void Cloth::init(MassPoint3D *start, int segments, float size, float singleMass) {
    // properties of rope:
    this->segments = segments;
	segsize = size / segments; // seglength without stress
	hardness = 4; // je dicker das seil desto fester :D
    this->totalMass = singleMass * (segments + 1) * (segments + 1);

	// adding points to cloth mesh
	int x = start->x;
	int z = start->z;
	int y = start->y;

	meshNet = new MassPoint3D**[segments];
	for(int i=0; i<=segments; i++){
		meshNet[i] = new MassPoint3D*[segments];
		for(int j=0; j<=segments; j++){

			// create a new MassPoint for this object:
			//MassPoint3D* spline = new MassPoint3D(x+i*segsize, y, z+j*segsize, singleMass);
			MassPoint3D* spline = new MassPoint3D(x+i*segsize, y, z+j*segsize, 8);

			
			// put them in pointList for physics effects (superclass compatibility!)
			pointList.push_front(spline); 

			// also put them into a matrix (makes creating springs much easier!)
			meshNet[i][j] = spline;

		}
	}


	// compute internal Springs:
	addSprings();

	// prepare texture:
    if (textureName ==  0) {
    	texture = Texture("cloth2.bmp", 512, 512);
    
    	glGenTextures(1, &textureName);
    	glBindTexture(GL_TEXTURE_2D, textureName);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.imagewidth, texture.imageheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.xData );
    }


#if CLOTH_BENDING == 1
    edgeBendingPoints = new MassPoint3D***[segments];
    middleBendingPoints = new MassPoint3D**[segments];
    bendingPointsCoding = new int*[segments];
    for (int i=0; i<segments; i++) {
        edgeBendingPoints[i] = new MassPoint3D**[segments];
        middleBendingPoints[i] = new MassPoint3D*[segments];
        bendingPointsCoding[i] = new int[segments];
        for (int j=0; j<segments; j++) {
            edgeBendingPoints[i][j] = new MassPoint3D*[2];
            bendingPointsCoding[i][j] = 0;
        }
    }
#endif 
}

MassPoint3D* Cloth::mesh(int i,int j) {
    if (i > segments || j > segments) return NULL;
    return meshNet[i][j];
}

/** Anchoring function ***********************/

void Cloth::anchor3() {
	// some hardcoded anchors
    meshNet[0][0]->setAnchor(true);
    meshNet[0][segments-1]->setAnchor(true);
    meshNet[segments-1][segments-1]->setAnchor(true);
}

// one row of anchors
void Cloth::anchorSide() {
    for (int i=0; i<segments+1; i++) {
        for (int j=0; j<segments+1; j++) {
            if (i == 0) meshNet[i][j]->setAnchor(true);
            else meshNet[i][j]->setAnchor(false);
        }
    }
}
    


// this was used back when we added points manually....
void Cloth::addSprings(){

	MassPoint3D *s, *e;

	// add horizontal springs
	for(int i=0; i<=segments; i++){
		for(int j=0; j<segments; j++){
			s = meshNet[i][j];
			e = meshNet[i][j+1];
			pushSpring(s, e, hardness);
		}
	}

	// add vertical springs
	for(int i=0; i<=segments; i++){
		for(int j=0; j<segments; j++){
			s = meshNet[j][i];
			e = meshNet[j+1][i];
			pushSpring(s, e, hardness);
		}
	}

	// add diagonal springs (optional)
	for(int i=0; i<segments; i++){
		for(int j=0; j<segments; j++){
			s = meshNet[i][j];
			e = meshNet[i+1][j+1];
			pushSpring(s, e, hardness);
		}
	}

	//cout << "Cloth with springs: " << springList.size() << endl;	
}



/* Arithmetic details *********/

float Cloth::veclength(float x1, float y1, float z1){
	return sqrt(x1*x1 + y1*y1 + z1*z1);
}

float Cloth::vecdot(float x1, float y1, float z1, float x2, float y2, float z2){	
	return x1*x2+y1*y2+z1*z2;
}

float Cloth::vecangle(float x1, float y1, float z1, float x2, float y2, float z2){
	float den = veclength(x1, y1, z1) * veclength(x2, y2, z2);
	float nom = vecdot(x1,y1,z1,  x2,y2,z2);
	float val = nom / den;
	return acos(val) * 180.0 / 3.14159265;
}




/** Bending of the cloth ************/

#if CLOTH_BENDING == 1
void Cloth::bendCorner(int i, int j, MassPoint3D* m) {
    middleBendingPoints[i][j] = m;
    bendingPointsCoding[i][j] |= 1;
}

void Cloth::bendEdge(int bi, int bj, MassPoint3D* m1, MassPoint3D* m2, int edge[2]) {
    // encoding at (i,j) is to be:
    // (0 i1 i2 i3 i4) in binary  where:
    // i1 is 1 if a point is on the edge (i,j) - (i,j+1)
    // i2 is 1 if a point is on the edge (i,j+1) - (i+1,j+1)
    // i3 is 1 if a point is on the edge (i+1,j+1) - (i+1,j)
    // i4 is 1 if a point is on the edge (i+1,j) - (i,j)
    /*MassPoint3D *ps[4] = { meshNet[bi][bj],
                           meshNet[bi][bj+1],
                           meshNet[bi+1][bj],
                           meshNet[bi+1][bj+1]};
    cout << "points (4 ps, 2 ms):\n";
    for (int i=0; i<4; i++) {
        cout << "      " << ps[i]->x << "," << ps[i]->y << "," <<ps[i]->z << endl;
    }
    MassPoint3D* ms[2] = {m1, m2};
    for (int i=0; i<2; i++) 
        cout<< "      " << ms[i]->x << "," << ms[i]->y << "," << ms[i]->z<<endl;
    int edge[2] = {0,0};
    int nearest[2] = {0,0}; // sec is to hold the second nearest pts
    float dist;
    float min, min2 = distance(ms[0], ms[1]);
    // for both masspoints, find on which edge they lie

    // find the 2 nearest points for both points
    // following macro is not exact, but it works to find the min (monotone fn)
#define mydist(a,b) (((a)->x-(b)->x) * ((a)->x - (b)->x) \
                        + ((a)->y - (b)->y) * ((a)->y - (b)->y) \
                        + ((a)->z - (b)->z) * ((a)->z - (b)->z))
    for (int i=0; i<2; i++) {
        min = min2 = 10000;
        for (int j=0; j<4; j++) {
            dist = mydist(ps[j], ms[i]);
            if (dist < min) {
                min = dist;
                nearest[1] = nearest[0];
                nearest[0] = j;
            }
            else if (dist < min2) {
                min2 = dist;
                nearest[1] = j;
            }
        }
    }
#undef mydist

    // find the nearest edge for both points
    for (int i=0; i<2; i++) {
        if ((ps[nearest[0]] == ps[0] && ps[nearest[1]] == ps[1]) || (ps[nearest[0]] == ps[1] && ps[nearest[1]] == ps[0]))
            edge[i] = 1;
        else if ((ps[nearest[0]] == ps[1] && ps[nearest[1]] == ps[3]) || (ps[nearest[0]] == ps[3] && ps[nearest[1]] == ps[1]))
            edge[i] = 2;
        else  if ((ps[nearest[0]] == ps[2] && ps[nearest[1]] == ps[3]) || (ps[nearest[0]] == ps[3] && ps[nearest[1]] == ps[2]))
            edge[i] = 3;
        else  if ((ps[nearest[0]] == ps[2] && ps[nearest[1]] == ps[0]) || (ps[nearest[0]] == ps[0] && ps[nearest[1]] == ps[2]))
            edge[i] = 4;
        else {
            cout << "Problem cloth.cpp::bendEdge: edge[0]=" << edge[0] << ", edge[1]="<<edge[1]<<", nearest[0]=" <<nearest[0]<< " nearest[1]="<<nearest[1]<<"\n";
            edge[0] = 1; // arb
        }
    }

    */
    // insert the bending points
    // specification is st. the edge index have to be in ascendent order
    if (edge[0] < edge[1]) {
        int i = edge[0];
        edge[0] = edge[1];
        edge[1] = i;
        MassPoint3D *p = m1;
        m1 = m2;
        m2 = p;
    }
    edgeBendingPoints[bi][bj][0] = m1;
    edgeBendingPoints[bi][bj][1] = m2;
    bendingPointsCoding[bi][bj] = (1 << edge[0]) | (1 << edge[1]);
    
    cout << "TODO bendEdge in Cloth.cpp\n";
}

void Cloth::clearBend(int i, int j) {
    int c = bendingPointsCoding[i][j];
    if (c & 1) {
        delete middleBendingPoints[i][j];
    }
    if (c > 1) { // edgeBendingPoints contains 2 elmts
        delete edgeBendingPoints[i][j][0];
        delete edgeBendingPoints[i][j][1];
    }
    bendingPointsCoding[i][j] = 0;
}
#endif // CLOTH_BENDING



/** Draw method ***********************
 * 
 */
void Cloth::draw(){

	#define SPRING_FRAME 0

	#if SPRING_FRAME
		WorldObject::draw();
	#else

    
	for(int i=0; i<segments; i++){
	for(int j=0; j<segments; j++){

		MassPoint3D* a = meshNet[i][j];
		MassPoint3D* b = meshNet[i][j+1];
		MassPoint3D* c = meshNet[i+1][j];
		MassPoint3D* d = meshNet[i+1][j+1];
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureName);

		glColor3f(1,1,1);
		float mcolor[] = { 1, 1, 1, 1.0f };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);	

#if CLOTH_BENDING == 0
		glBegin(GL_POLYGON);
			glTexCoord2d(0.0, 0.0);
			glVertex3f(a->x, a->y, a->z);
			glTexCoord2d(1.0, 0.0);
			glVertex3f(b->x, b->y, b->z);
			glTexCoord2d(0.0, 1.0);
			glVertex3f(c->x, c->y, c->z);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.0, 0.0);
			glVertex3f(d->x, d->y, d->z);
			glTexCoord2d(1.0, 0.0);
			glVertex3f(b->x, b->y, b->z);
			glTexCoord2d(0.0, 1.0);
			glVertex3f(c->x, c->y, c->z);
		glEnd();
#else 

#define xratio_(p) (((p)->x - a->x) / (d->x - a->x))
#define yratio_(p) (((p)->y - a->y) / (d->y - a->y))
#define coords(p) (p)->x, (p)->y, (p)->z
        int n = bendingPointsCoding[i][j];
        // to draw a pyramid:
        float xratioTop, yratioTop;
        MassPoint3D* top;
        if (n & 1) {
            top = middleBendingPoints[i][j];
            xratioTop = xratio_(top);
            yratioTop = yratio_(top);
        }

        // for the case of an edge:
        float xratio1, yratio1, xratio2, yratio2;
        MassPoint3D *b1, *b2;
        if (n > 1) {
            b1 = *edgeBendingPoints[i][j];
            b2 = *(edgeBendingPoints[i][j] + 1);
        }
        //switch (n) {
        //case 0: 
        if (n == 0) {// draw two triangles
    		glBegin(GL_POLYGON);
    			glTexCoord2d(0.0, 0.0);
    			glVertex3f(coords(a));
    			glTexCoord2d(1.0, 0.0);
    			glVertex3f(coords(b));
    			glTexCoord2d(0.0, 1.0);
    			glVertex3f(coords(c));
    		glEnd();
    		glBegin(GL_POLYGON);
    			glTexCoord2d(1.0, 1.0);
    			glVertex3f(coords(d));
    			glTexCoord2d(0.0, 1.0);
    			glVertex3f(coords(b));
    			glTexCoord2d(1.0, 0.0);
    			glVertex3f(coords(c));
    		glEnd();
        }
        //    break;
        //case 1: 
        if (n == 1) { // simple pyramid
            // a-b-top
            glBegin(GL_POLYGON);
    			glTexCoord2d(0.0, 0.0);
    			glVertex3f(coords(a));
    			glTexCoord2d(0.0, 1.0);
    			glVertex3f(coords(b));
    			glTexCoord2d(xratioTop, yratioTop);
    			glVertex3f(coords(top));
    		glEnd();
            // b-d-top
            glBegin(GL_POLYGON);
                glTexCoord2d(0.0, 1.0);
                glVertex3f(coords(b));
                glTexCoord2d(1.0, 1.0);
                glVertex3f(coords(d));
                glTexCoord2d(xratioTop, yratioTop);
                glVertex3f(coords(top));
            glEnd();
            // c-d-top
            glBegin(GL_POLYGON);
    	    	glTexCoord2d(1.0, 0.0);
    	    	glVertex3f(coords(c));
    	    	glTexCoord2d(1.0, 1.0);
    	    	glVertex3f(coords(d));
    	    	glTexCoord2d(xratioTop, yratioTop);
    	    	glVertex3f(coords(top));
    	    glEnd();
            // a-c-top
            glBegin(GL_POLYGON);
    			glTexCoord2d(0.0, 0.0);
    			glVertex3f(coords(a));
    			glTexCoord2d(1.0, 0.0);
    			glVertex3f(coords(c));
    			glTexCoord2d(xratioTop, yratioTop);
    			glVertex3f(coords(top));
    		glEnd();
        }
        
        // The following cases are for when the tile is divided by an edge,
        // we then want to fill 4 or 5 triangles, depending on
        // the position of the bending points. We assume b1 is before b2
        // in the 1234 order described below.
        // encoding at (i,j) is to be:
        // (0 i1 i2 i3 i4) in binary  where:
        // i1 is 1 if a point is on the edge (i,j) - (i,j+1) = a-b
        // i2 is 1 if a point is on the edge (i,j+1) - (i+1,j+1) = b-d
        // i3 is 1 if a point is on the edge (i+1,j+1) - (i+1,j) = d-c
        // i4 is 1 if a point is on the edge (i+1,j) - (i,j) = c-a


        else if (n & 1) {
            // draw a pyramid to the bending point with 
            bool usedB1 = false;
            // a-b-top
            if (n & 2) {
                xratio1 = 0;
                yratio1 = yratio_(b1);
                glBegin(GL_POLYGON);
    		    	glTexCoord2d(0.0, 0.0);
    		    	glVertex3f(coords(a));
    		    	glTexCoord2d(xratio1, yratio1);
    		    	glVertex3f(coords(b1));
    		    	glTexCoord2d(xratioTop, yratioTop);
    		    	glVertex3f(coords(top));
    		    glEnd();
                glBegin(GL_POLYGON);
    		    	glTexCoord2d(xratio1, yratio1);
    		    	glVertex3f(coords(b1));
    		    	glTexCoord2d(0.0, 1.0);
    		    	glVertex3f(coords(b));
    		    	glTexCoord2d(xratioTop, yratioTop);
    		    	glVertex3f(coords(top));
    		    glEnd();
                usedB1 = true;
            }
            else {
                glBegin(GL_POLYGON);
    		    	glTexCoord2d(0.0, 0.0);
    		    	glVertex3f(coords(a));
    		    	glTexCoord2d(0.0, 1.0);
    		    	glVertex3f(coords(b));
    		    	glTexCoord2d(xratioTop, yratioTop);
    		    	glVertex3f(coords(top));
    		    glEnd();
            }
            // b-d-top
            if (n & 4) {
                MassPoint3D *tmp;
                if (!usedB1) {
                    tmp = b1;
                    usedB1 = true;
                }
                else tmp = b2;

                float xratioT = xratio_(tmp),
                      yratioT = yratio_(tmp);
                glBegin(GL_POLYGON);
    		    	glTexCoord2d(0.0, 1.0);
    		    	glVertex3f(coords(b));
    		    	glTexCoord2d(xratioT, yratioT);
    		    	glVertex3f(coords(tmp));
    		    	glTexCoord2d(xratioTop, yratioTop);
    		    	glVertex3f(coords(top));
    		    glEnd();
                glBegin(GL_POLYGON);
    		    	glTexCoord2d(xratioT, yratioT);
    		    	glVertex3f(coords(tmp));
    		    	glTexCoord2d(1.0, 1.0);
    		    	glVertex3f(coords(d));
    		    	glTexCoord2d(xratioTop, yratioTop);
    		    	glVertex3f(coords(top));
    		    glEnd();
            }
            else {
                glBegin(GL_POLYGON);
    		    	glTexCoord2d(0.0, 1.0);
    		    	glVertex3f(coords(b));
    		    	glTexCoord2d(1.0, 1.0);
    		    	glVertex3f(coords(d));
    		    	glTexCoord2d(xratioTop, yratioTop);
    		    	glVertex3f(coords(top));
    		    glEnd();
            }
            // c-d-top
            if (n & 8) {
                MassPoint3D *tmp;
                if (!usedB1) {
                    tmp = b1;
                    usedB1 = true;
                }
                else tmp = b2;
                float xratioT = xratio_(tmp),
                      yratioT = yratio_(tmp);
                glBegin(GL_POLYGON);
    		    	glTexCoord2d(1.0, 0.0);
    		    	glVertex3f(coords(c));
    		    	glTexCoord2d(xratioT, yratioT);
    		    	glVertex3f(coords(tmp));
    		    	glTexCoord2d(xratioTop, yratioTop);
    		    	glVertex3f(coords(top));
    		    glEnd();
                glBegin(GL_POLYGON);
    		    	glTexCoord2d(xratioT, yratioT);
    		    	glVertex3f(coords(tmp));
    		    	glTexCoord2d(1.0, 1.0);
    		    	glVertex3f(coords(d));
    		    	glTexCoord2d(xratioTop, yratioTop);
    		    	glVertex3f(coords(top));
    		    glEnd();
            }
            else {
                glBegin(GL_POLYGON);
    		    	glTexCoord2d(1.0, 0.0);
    		    	glVertex3f(coords(c));
    		    	glTexCoord2d(1.0, 1.0);
    		    	glVertex3f(coords(d));
    		    	glTexCoord2d(xratioTop, yratioTop);
    		    	glVertex3f(coords(top));
    		    glEnd();
            }
            // a-c-top
            if (n & 16) {
                glBegin(GL_POLYGON);
    		    	glTexCoord2d(0.0, 0.0);
    		    	glVertex3f(coords(a));
    		    	glTexCoord2d(xratio2, yratio2);
    		    	glVertex3f(coords(b2));
    		    	glTexCoord2d(xratioTop, yratioTop);
    		    	glVertex3f(coords(top));
    		    glEnd();
                glBegin(GL_POLYGON);
    		    	glTexCoord2d(xratio2, yratio2);
    		    	glVertex3f(coords(b2));
    		    	glTexCoord2d(1.0, 0.0);
    		    	glVertex3f(coords(c));
    		    	glTexCoord2d(xratioTop, yratioTop);
    		    	glVertex3f(coords(top));
    		    glEnd();
            }
            else {
                glBegin(GL_POLYGON);
    		    	glTexCoord2d(0.0, 0.0);
    		    	glVertex3f(coords(a));
    		    	glTexCoord2d(1.0, 0.0);
    		    	glVertex3f(coords(c));
    		    	glTexCoord2d(xratioTop, yratioTop);
    		    	glVertex3f(coords(top));
    		    glEnd();
            }
        }
        //    break;

                    // TODO when 2 rectangles, still draw 4 triangles!
        //case 6: 
        // i1, i2
        else if (n == 6) {
            xratio1 = 0;
            yratio1 = yratio_(b1);
            xratio2 = xratio_(b2);
            yratio2 = 1;
            // b - b1 - b2
            glBegin(GL_POLYGON);
    			glTexCoord2d(0,1);
    			glVertex3f(coords(b));
    			glTexCoord2d(xratio1, yratio1);
    			glVertex3f(coords(b1));
    			glTexCoord2d(xratio2, yratio2);
    			glVertex3f(coords(b2));
    		glEnd();
            // a - b1 - c
            glBegin(GL_POLYGON);
    			glTexCoord2d(0,0);
    			glVertex3f(coords(a));
    			glTexCoord2d(xratio1, yratio1);
    			glVertex3f(coords(b1));
    			glTexCoord2d(1,0);
    			glVertex3f(coords(c));
    		glEnd();
            // c - b1 - b2
            glBegin(GL_POLYGON);
    			glTexCoord2d(1,0);
    			glVertex3f(coords(c));
    			glTexCoord2d(xratio1, yratio1);
    			glVertex3f(coords(b1));
    			glTexCoord2d(xratio2, yratio2);
    			glVertex3f(coords(b2));
    		glEnd();
            // b2 - c - d
            glBegin(GL_POLYGON);
    			glTexCoord2d(xratio2, yratio2);
    			glVertex3f(coords(b2));
    			glTexCoord2d(1, 0);
    			glVertex3f(coords(c));
    			glTexCoord2d(1,1);
    			glVertex3f(coords(d));
    		glEnd();
        }
        //    break;
        //case 10:
        // i1, i3
        else if (n == 10) {
            xratio1 = 0;
            yratio1 = yratio_(b1);
            xratio2 = 1;
            yratio2 = yratio_(b2);
            // a - c - b2 - b1
            glBegin(GL_POLYGON);
    			glTexCoord2d(0,0);
    			glVertex3f(coords(a));
    			glTexCoord2d(1,0);
    			glVertex3f(coords(c));
    			glTexCoord2d(xratio2, yratio2);
    			glVertex3f(coords(b2));
    			glTexCoord2d(xratio1, yratio1);
    			glVertex3f(coords(b1));
            glEnd();
            // b - d - b2 - b1
            glBegin(GL_POLYGON);
    			glTexCoord2d(0,1);
    			glVertex3f(coords(b));
    			glTexCoord2d(1,1);
    			glVertex3f(coords(d));
    			glTexCoord2d(xratio2, yratio2);
    			glVertex3f(coords(b2));
    			glTexCoord2d(xratio1, yratio1);
    			glVertex3f(coords(b1));
            glEnd();
        }
        //    break;
        //case 18: 
        // i1, i4
        else if (n == 18) {
            xratio1 = 0;
            yratio1 = yratio_(b1);//(b1->y - a->y) / (d->y - a->y);
            xratio2 = xratio_(b2);//(b2->x - a->x) / (d->x - a->x);
            yratio2 = 0;
            // a - b1 - b2
            glBegin(GL_POLYGON);
    			glTexCoord2d(0,0);
    			glVertex3f(coords(a));
    			glTexCoord2d(xratio1, yratio1);
    			glVertex3f(coords(b1));
    			glTexCoord2d(xratio2, yratio2);
    			glVertex3f(coords(b2));
    		glEnd();
            // b - b1 - d
            glBegin(GL_POLYGON);
    			glTexCoord2d(0,1);
    			glVertex3f(coords(b));
    			glTexCoord2d(xratio1, yratio1);
    			glVertex3f(coords(b1));
    			glTexCoord2d(1,1);
    			glVertex3f(coords(d));
    		glEnd();
            // d - b1 - b2
            glBegin(GL_POLYGON);
    			glTexCoord2d(1,1);
    			glVertex3f(coords(d));
    			glTexCoord2d(xratio1, yratio1);
    			glVertex3f(coords(b1));
    			glTexCoord2d(xratio2, yratio2);
    			glVertex3f(coords(b2));
    		glEnd();
            // b2 - c - d
            glBegin(GL_POLYGON);
    			glTexCoord2d(xratio2, yratio2);
    			glVertex3f(coords(b2));
    			glTexCoord2d(1, 0);
    			glVertex3f(coords(c));
    			glTexCoord2d(1,1);
    			glVertex3f(coords(d));
    		glEnd();
        }
        //    break;
        //case 12:
        // i2, i3
        else if (n == 12) {
            xratio1 = xratio_(b1);
            yratio1 = 1;
            xratio2 = 1;
            yratio2 = yratio_(b2);
            // a - b1 - b2
            glBegin(GL_POLYGON);
    			glTexCoord2d(0,0);
    			glVertex3f(coords(a));
    			glTexCoord2d(xratio1, yratio1);
    			glVertex3f(coords(b1));
    			glTexCoord2d(xratio2, yratio2);
    			glVertex3f(coords(b2));
    		glEnd();
            // a - b1 - b
            glBegin(GL_POLYGON);
    			glTexCoord2d(0,0);
    			glVertex3f(coords(a));
    			glTexCoord2d(xratio1, yratio1);
    			glVertex3f(coords(b1));
    			glTexCoord2d(0,1);
    			glVertex3f(coords(b));
    		glEnd();
            // d - b1 - b2
            glBegin(GL_POLYGON);
    			glTexCoord2d(1,1);
    			glVertex3f(coords(d));
    			glTexCoord2d(xratio1, yratio1);
    			glVertex3f(coords(b1));
    			glTexCoord2d(xratio2, yratio2);
    			glVertex3f(coords(b2));
    		glEnd();
            // b2 - c - a
            glBegin(GL_POLYGON);
    			glTexCoord2d(xratio2, yratio2);
    			glVertex3f(coords(b2));
    			glTexCoord2d(1, 0);
    			glVertex3f(coords(c));
    			glTexCoord2d(0,0);
    			glVertex3f(coords(a));
    		glEnd();
        }
        //    break;
        //case 20:
        // i2, i4
        else if (n == 20) {
            xratio1 = xratio_(b1);
            yratio1 = 0;
            xratio2 = xratio_(b2);
            yratio2 = 1;
            // a - b - b1 - b2
            glBegin(GL_POLYGON);
    			glTexCoord2d(0,0);
    			glVertex3f(coords(a));
    			glTexCoord2d(0,1);
    			glVertex3f(coords(b));
    			glTexCoord2d(xratio2, yratio2);
    			glVertex3f(coords(b2));
    			glTexCoord2d(xratio1, yratio1);
    			glVertex3f(coords(b1));
            glEnd();
            // c - d - b1 - b2
            glBegin(GL_POLYGON);
    			glTexCoord2d(1,0);
    			glVertex3f(coords(c));
    			glTexCoord2d(1,1);
    			glVertex3f(coords(d));
    			glTexCoord2d(xratio1, yratio1);
    			glVertex3f(coords(b1));
    			glTexCoord2d(xratio2, yratio2);
    			glVertex3f(coords(b2));
            glEnd();
        }
        //    break;
        //case 24:
        // i3, i4
        else if (n == 24) {
            xratio1 = 1;
            yratio1 = yratio_(b1);
            xratio2 = xratio_(b2);
            yratio2 = 0;
            // b - b1 - b2
            glBegin(GL_POLYGON);
    			glTexCoord2d(0,1);
    			glVertex3f(coords(b));
    			glTexCoord2d(xratio1, yratio1);
    			glVertex3f(coords(b1));
    			glTexCoord2d(xratio2, yratio2);
    			glVertex3f(coords(b2));
    		glEnd();
            // a - b2 - b
            glBegin(GL_POLYGON);
    			glTexCoord2d(0,0);
    			glVertex3f(coords(a));
    			glTexCoord2d(xratio2, yratio2);
    			glVertex3f(coords(b2));
    			glTexCoord2d(0,1);
    			glVertex3f(coords(b));
    		glEnd();
            // c - b1 - b2
            glBegin(GL_POLYGON);
    			glTexCoord2d(1,0);
    			glVertex3f(coords(c));
    			glTexCoord2d(xratio1, yratio1);
    			glVertex3f(coords(b1));
    			glTexCoord2d(xratio2, yratio2);
    			glVertex3f(coords(b2));
    		glEnd();
            // b1 - b - d
            glBegin(GL_POLYGON);
    			glTexCoord2d(xratio2, yratio2);
    			glVertex3f(coords(b2));
    			glTexCoord2d(1, 0);
    			glVertex3f(coords(b));
    			glTexCoord2d(0,0);
    			glVertex3f(coords(d));
    		glEnd();
           // break;
        }
        
        //break;
        //}
#endif
		glDisable(GL_TEXTURE_2D);
	} // end for j
	} // end for i

	#endif

}
