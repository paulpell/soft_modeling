#include "ClothOnBox.h"
#include "Vector3D.h"
#include "MassPoint3D.h"

ClothOnBox::ClothOnBox(){};

ClothOnBox::ClothOnBox (float x, float y, float z) {
        int segments = 10;
        float size = 5;
		//box = new Cube(x,y,z,size);
        box = NULL;
        nail = new MassPoint3D(x + size/2, y + size, z+ size/2);
		cloth = new Cloth(new MassPoint3D(x - size/2, y + size + 1, z-size/2), segments, 2*size);
		cloth->applyGlobalForce(new Force(0,-.5,0)); // gravity shall be
        /*
        // a force will potentially be exerted on every point 
        forces = new Force**[segments+1];
        for (int i=0; i<segments+1; ++i) {
            forces[i] = new Force*[segments+1];
            for (int j=0; j<segments+1; ++j) {
                Force *f = new Force(0,0,0);
                cloth->mesh(i,j)->addForce(f);
                forces[i][j] = f;
            }
        }
        pushObject(box);
        */
        pushObject(cloth);
}

void ClothOnBox::timeStep(float dt) {
    // box is not moving
    int segments = cloth->getSegments();
    MassPoint3D *m;
    // a pair of indices is at the same pos in both lists
    list<int> colliding_x, colliding_y;

    cloth->timeStep(dt);



    /*
    // now check for collision detection
    for (int i=0; i<=segments; i++) {
        for (int j=0; j<= segments; j++) {
            m = cloth->mesh(i,j);
            if (box->isPointInside(m)) {
                box->collide(m, forces[i][j], dt);
                m->setAnchor(true);
                colliding_x.push_front(i);
                colliding_y.push_front(j);
            }
            else {
                forces[i][j]->reset();
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

        if (box->isPointInBaseSquare(cloth->mesh(*itx, *ity))) {
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
        }
    }
    */
}

void ClothOnBox::draw() {
	if (box)
        box->draw();
    else {
//        glBegin(GL_POINTS);
        glPointSize(50);
        glColor3f(.9, .1, .2);
        glVertex3f(nail->x, nail->y, nail->z);
 //       glEnd();
    }
    cloth->draw();
}
