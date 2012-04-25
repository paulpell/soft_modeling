#include "ClothOnBox.h"
#include "arith.h"

// in the old way, a force was exerted by the box on the nearby massPoints
#define COLLIDE_OLD_WAY 0


ClothOnBox::ClothOnBox(){};

ClothOnBox::ClothOnBox (float x, float y, float z) {
        int segments = 5;
        float size = 5;
        box = NULL;
		cloth = new Cloth(new MassPoint3D(x - size/2, y + size + 3, z-size/2), segments, 2*size);

#define pt(i,j) (cloth->mesh(i,j))
#if CLOTHONBOX_NAIL == 1
        int nail_i=size/2, nail_j = size / 2;
        MassPoint3D *a = pt(nail_i, nail_j),
                    *b = pt(nail_i, nail_j+1),
                    *c = pt(nail_i+1, nail_j),
                    *d = pt(nail_i+1, nail_j+1);
        float nx = (a->x + d->x) / 2.;
        float ny = (a->y + d->y) / 2.;
        float nz = (a->z + d->z) / 2.;

        // setup a nail
        nail = new MassPoint3D(nx, ny, nz, 1);
        nail->setAnchor(true);

        float hardness = 10;
        nailSprings[0] = new Spring(nail, a, hardness);
        nailSprings[1] = new Spring(nail, b, hardness);
        nailSprings[2] = new Spring(nail, c, hardness);
        nailSprings[3] = new Spring(nail, d, hardness);
        cloth->bendCorner(nail_i, nail_j, nail);
#else
        basex = x;
        basey = y;
        basez = z;
        this->size = size;
		box = new Cube(x,y,z,size);
        pushObject(box);


#if COLLIDE_OLD_WAY == 1
        // a force will potentially be exerted on every point by the cube
        forces = new Force**[segments+1];
        for (int i=0; i<segments+1; ++i) {
            forces[i] = new Force*[segments+1];
            for (int j=0; j<segments+1; ++j) {
                Force *f = new Force(0,0,0);
                cloth->mesh(i,j)->addForce(f);
                forces[i][j] = f;
            }
        }
#endif // COLLIDE_OLD_WAY

#endif // CLOTHONBOX_NAIL
		cloth->applyGlobalForce(new Force(0,-.5,0)); // gravity shall be
        pushObject(cloth);
}


/*** Time step function, animates the cloth ****************/

void ClothOnBox::timeStep(float dt) {
    // box is not moving
    int segments = cloth->getSegments();
    // a pair of indices is at the same pos in both lists
    list<int> colliding_x, colliding_y;

    // first compute the force
#if CLOTHONBOX_NAIL == 1
    for (int i=0; i<4; i++) {
        nailSprings[i]->timeStep(dt);
    }
#endif

    cloth->timeStep(dt);



#if CLOTHONBOX_NAIL == 1

#else // the cloth interacts with the box
    
#if COLLIDE_OLD_WAY == 1
    // in this simulation, the box exerts a force on every nearby mass point

    MassPoint3D *m;
    // now check for collision detection
    for (int i=0; i<=segments; i++) {
        for (int j=0; j<= segments; j++) {
            m = cloth->mesh(i,j);
            if (box->isPointInside(m)) {
                // is p is in the box, we let the box handle
                box->collide(m, forces[i][j], dt);
                //m->setAnchor(true);
                // save info of which masspoints did collide
                colliding_x.push_front(i);
                colliding_y.push_front(j);
            }
            else if (!box->isPointInActiveArea(m)) {
                forces[i][j]->reset();
            }
            else {
                forces[i][j]->scale(.5);
            }
        }
    }

    
    // one more loop, to avoid cloth inside cube
    list<int>::iterator
        itx  = colliding_x.begin(),
        endx = colliding_x.end(),
        ity  = colliding_y.begin(),
        endy = colliding_y.end();
    for (; itx != endx; itx++, ity++) {
 
       //if (box->isPointInBaseSquare(cloth->mesh(*itx, *ity))) {
             if (*itx > 0) {
                 if (!box->isPointInBaseSquare(cloth->mesh(*itx-1, *ity))) {
                     cloth->mesh(*itx-1, *ity)->y = box->maxY();
                 }
             }
             if (*itx < segments) {
                 if (!box->isPointInBaseSquare(cloth->mesh(*itx+1, *ity))) {
                     cloth->mesh(*itx+1, *ity)->y = box->maxY();
                 }
             }
             if (*ity > 0) {
                 if (!box->isPointInBaseSquare(cloth->mesh(*itx, *ity-1))) {
                     cloth->mesh(*itx, *ity-1)->y = box->maxY();
                 }
             }
             if (*ity < segments) {
                 if (!box->isPointInBaseSquare(cloth->mesh(*itx, *ity+1))) {
                     cloth->mesh(*itx, *ity+1)->y = box->maxY();
                 }
             }
       //}
     }


#else // COLLIDE_OLD_WAY

#define boxpt(i,j,k) box->mesh[i][j][k] 

    // let the points collide if they enter the box
    for (int i=0; i<segments+1; i++) {
        for (int j=0; j<segments+1; j++) {
            if (box->isPointInside(pt(i,j))) {
                box->collide(pt(i,j), NULL, dt);
            }
        }
    }

    float mass = 4;
    MassPoint3D *m1 = new MassPoint3D(0,0,0, mass),
                *m2 = new MassPoint3D(0,0,0, mass);
    for (int i=0; i<segments; i++) {
        for (int j=0; j<segments; j++) {

            cloth->clearBend(i,j);


            // let some helpers be defined here
            MassPoint3D *ms[4] = 
                    {pt(i,j), pt(i,j+1), pt(i+1,j), pt(i+1,j+1)};

            MassPoint3D *a = pt(i,j), *b = pt(i, j+1),
                      *c = pt(i+1, j), *d = pt(i+1, j+1);

            int pos1 = box->relpos(a),
                pos2 = box->relpos(d);

            // depending on the relative position of both points wrt the cube, 
            // we may have to insert one or two bending points to the cloth tile.
            // The logic for that follows, it's not very readable but well....
            // See Cube.c for the relpos encoding.

            // one point is outside with lower x
            if ((pos1 & 1) ^ (pos2 & 1)) {
                // one p with lower y
                if ((pos1 & 4) ^ (pos2 & 4)) {
                    if ((pos1 & 16) ^ (pos2 & 16)) {
                        m1 = new MassPoint3D(basex, basey, basez, mass);
                        cloth->bendCorner(i,j,m1);
                    }
                    else if ((pos1 & 32) ^ (pos2 & 32)) {
                        m1 = new MassPoint3D(basex, basey, basez + size, mass);
                        cloth->bendCorner(i,j,m1);
                    }
                    else if (!(pos1 & 16 || pos1 & 32 || pos2 & 16 || pos2 & 32)) { // both inside the box wrt to z
                        //m1 = new MassPoint3D(basex, basey, (a->z + b->z) /2, mass);
                        //m2 = new MassPoint3D(basex, basey, (c->z + d->z) / 2, mass);
                        m1 = new MassPoint3D(0,0,0,mass);
                        m2 = new MassPoint3D(0,0,0,mass);
                        LineLineIntersect(boxpt(0,0,0), boxpt(0,0,1), a, c, m1);
                        LineLineIntersect(boxpt(0,0,0), boxpt(0,0,1), b, d, m2);
                        cloth->bendEdge(i, j, m1, m2);
                    }
                }
                // one p with higher y
                else if ((pos1 & 8) ^ (pos2 & 8)) {
                    if ((pos1 & 16) ^ (pos2 & 16)) {
                        m1 = new MassPoint3D(basex, basey + size, basez, mass);
                        cloth->bendCorner(i,j,m1);
                    }
                    else if ((pos1 & 32) ^ (pos2 & 32)) {
                        m1 = new MassPoint3D(basex, basey + size, basez +size, mass);
                        cloth->bendCorner(i,j,m1);
                    }
                    else if (!(pos1 & 16 || pos1 & 32 || pos2 & 16 || pos2 & 32)){ // both inside the box wrt to z
                        //m1 = new MassPoint3D(basex, basey + size, (a->z + b->z) /2, mass);
                        //m2 = new MassPoint3D(basex, basey + size, (c->z + d->z) / 2, mass);
                        m1 = new MassPoint3D(0,0,0,mass);
                        m2 = new MassPoint3D(0,0,0,mass);
                        LineLineIntersect(boxpt(0,1,0), boxpt(0,1,1), a, c, m1);
                        LineLineIntersect(boxpt(0,1,0), boxpt(0,1,1), b, d, m2);
                        cloth->bendEdge(i, j, m1, m2);
                    }
                }
                else if (!(pos1 & 4 || pos1 & 8 || pos2 & 4 || pos2 & 8)){ // both inside the box wrt y
                    if ((pos1 & 16) ^ (pos2 & 16)) {
                        m1 = new MassPoint3D(basex, basey + size, basez, mass);
                        cloth->bendCorner(i,j,m1);
                    }
                    else if ((pos1 & 32) ^ (pos2 & 32)) {
                        m1 = new MassPoint3D(basex, basey + size, basez +size, mass);
                        cloth->bendCorner(i,j,m1);
                    }
                    else if (!(pos1 & 16 || pos1 & 32 || pos2 & 16 || pos2 & 32)){ // both inside the box wrt to z
                        //m1 = new MassPoint3D(basex, basey + size, (a->z + b->z) /2, mass);
                        //m2 = new MassPoint3D(basex, basey + size, (c->z + d->z) / 2, mass);
                        //cloth->bendEdge(i, j, m1, m2);
                    }
                }
            }
            else if ((pos1 & 2) ^ (pos2 & 2)) { // one p with higher x
                // one p with lower y
                if ((pos1 & 4) ^ (pos2 & 4)) {
                    if ((pos1 & 16) ^ (pos2 & 16)) {
                        m1 = new MassPoint3D(basex + size, basey, basez, mass);
                        cloth->bendCorner(i,j,m1);
                    }
                    else if ((pos1 & 32) ^ (pos2 & 32)) {
                        m1 = new MassPoint3D(basex + size, basey, basez + size, mass);
                        cloth->bendCorner(i,j,m1);
                    }
                    else if (!(pos1 & 16 || pos1 & 32 || pos2 & 16 || pos2 & 32)){ // both inside the box wrt to z
                        //m1 = new MassPoint3D(basex + size, basey, (a->z + b->z) /2, mass);
                        //m2 = new MassPoint3D(basex + size, basey, (c->z + d->z) / 2, mass);
                        m1 = new MassPoint3D(0,0,0,mass);
                        m2 = new MassPoint3D(0,0,0,mass);
                        LineLineIntersect(boxpt(1,0,0), boxpt(1,0,1), a, c, m1);
                        LineLineIntersect(boxpt(1,0,0), boxpt(1,0,1), b, d, m2);
                        cloth->bendEdge(i, j, m1, m2);
                    }
                }
                // one p with higher y
                else if ((pos1 & 8) ^ (pos2 & 8)) {
                    if ((pos1 & 16) ^ (pos2 & 16)) {
                        m1 = new MassPoint3D(basex + size, basey + size, basez, mass);
                        cloth->bendCorner(i,j,m1);
                    }
                    else if ((pos1 & 32) ^ (pos2 & 32)) {
                        m1 = new MassPoint3D(basex + size, basey + size, basez +size, mass);
                        cloth->bendCorner(i,j,m1);
                    }
                    else if (!(pos1 & 16 || pos1 & 32 || pos2 & 16 || pos2 & 32)){ // both inside the box wrt to z
                        //m1 = new MassPoint3D(basex + size, basey + size, (a->z + b->z) /2, mass);
                        //m2 = new MassPoint3D(basex + size, basey + size, (c->z + d->z) / 2, mass);
                        m1 = new MassPoint3D(0,0,0,mass);
                        m2 = new MassPoint3D(0,0,0,mass);
                        LineLineIntersect(boxpt(1,1,0), boxpt(1,1,1), a, c, m1);
                        LineLineIntersect(boxpt(1,1,0), boxpt(1,1,1), b, d, m2);
                        cloth->bendEdge(i, j, m1, m2);
                    }
                }
                else if (!(pos1 & 4 || pos1 & 8 || pos2 & 4 || pos2 & 8)){ // both inside the box wrt y
                    if ((pos1 & 16) ^ (pos2 & 16)) {
                        m1 = new MassPoint3D(basex + size, basey + size, basez, mass);
                        cloth->bendCorner(i,j,m1);
                    }
                    else if ((pos1 & 32) ^ (pos2 & 32)) {
                        m1 = new MassPoint3D(basex + size, basey + size, basez +size, mass);
                        cloth->bendCorner(i,j,m1);
                    }
                    else if (!(pos1 & 16 || pos1 & 32 || pos2 & 16 || pos2 & 32)) { // both inside the box wrt to z
                        //m1 = new MassPoint3D(basex + size, basey + size, (a->z + b->z) /2, mass);
                        //m2 = new MassPoint3D(basex + size, basey + size, (c->z + d->z) / 2, mass);
                        //cloth->bendEdge(i, j, m1, m2);
                    }
                }
            }

            else if(!(pos1 & 1 || pos1 & 2 || pos2 & 1 || pos2 & 2)) { // both inside the box wrt x
                if (((pos1 & 4) ^ (pos2 & 4)) && ((pos1 & 16) ^ (pos2 & 16))) {
                    //m1 = new MassPoint3D((a->x + b->x) / 2., basey, basez, mass);
                    //m2 = new MassPoint3D((c->x + d->x) / 2., basey, basez, mass);
                    m1 = new MassPoint3D(0,0,0,mass);
                    m2 = new MassPoint3D(0,0,0,mass);
                    LineLineIntersect(boxpt(0,0,0), boxpt(1,0,0), a, c, m1);
                    LineLineIntersect(boxpt(0,0,0), boxpt(1,0,0), b, d, m2);
                    cloth->bendEdge(i, j, m1, m2);
                }
                else if (((pos1 & 4) ^ (pos2 & 4)) && ((pos1 & 32) ^ (pos2 & 32))) {
                    //m1 = new MassPoint3D((a->x + b->x) / 2., basey, basez + size, mass);
                    //m2 = new MassPoint3D((c->x + d->x) / 2., basey, basez + size, mass);
                    m1 = new MassPoint3D(0,0,0,mass);
                    m2 = new MassPoint3D(0,0,0,mass);
                    LineLineIntersect(boxpt(0,0,1), boxpt(1,0,1), a, c, m1);
                    LineLineIntersect(boxpt(0,0,1), boxpt(1,0,1), b, d, m2);
                    cloth->bendEdge(i, j, m1, m2);
                }
                else if (((pos1 & 8) ^ (pos2 & 8)) && ((pos1 & 16) ^ (pos2 & 16))) {
                    //m1 = new MassPoint3D((a->x + b->x) / 2., basey + size, basez, mass);
                    //m2 = new MassPoint3D((c->x + d->x) / 2., basey + size, basez, mass);
                    m1 = new MassPoint3D(0,0,0,mass);
                    m2 = new MassPoint3D(0,0,0,mass);
                    LineLineIntersect(boxpt(0,1,0), boxpt(1,1,0), a, c, m1);
                    LineLineIntersect(boxpt(0,1,0), boxpt(1,1,0), b, d, m2);
                    cloth->bendEdge(i, j, m1, m2);
                }
                else if (((pos1 & 8) ^ (pos2 & 8)) && ((pos1 & 32) ^ (pos2 & 32))) {
                    //m1 = new MassPoint3D((a->x + b->x) / 2., basey + size, basez + size, mass);
                    //m2 = new MassPoint3D((c->x + d->x) / 2., basey + size, basez + size, mass);
                    m1 = new MassPoint3D(0,0,0,mass);
                    m2 = new MassPoint3D(0,0,0,mass);
                    LineLineIntersect(boxpt(0,1,1), boxpt(1,1,1), a, c, m1);
                    LineLineIntersect(boxpt(0,1,1), boxpt(1,1,1), b, d, m2);
                    cloth->bendEdge(i, j, m1, m2);
                }
            }
        }
    }

#endif // COLLIDE_OLD_WAY

#endif // CLOTHONBOX_NAIL
}

void ClothOnBox::draw() {
#if CLOTHONBOX_NAIL == 1
    nail->draw();
#else
    box->draw();
#endif

    cloth->draw();
}
