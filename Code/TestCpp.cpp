//============================================================================
// Name        : 
// Author      : Paul and Philipp
// Version     :
// Description : Modeling Soft Objects with OpenGL
//============================================================================

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>

#include "Cube3D.h"
#include "Camera.h"
#include "Vertex3D.h"
#include "Vector3D.h"
#include "Surface3D.h"
#include "Texture.h"
#include "Keyboard.h"
#include "World3D.h"
#include "MassPoint3D.h"
#include "Spring.h"

#include "Cloth.h"
#include "Jelly.h"


//#include "WorldObject.h"
#include "Rope.h"

Camera myCamera;


// Objects in the world:
Surface3D theFloor;
Rope myRope;
Cloth myCloth;
Jelly myJelly;
 

int moving, startx, starty;

void init(void) {
	glClearColor (0.1, 0.2, 0.7, 0.0);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_POINT_SMOOTH);

	// Set camera:
	myCamera = Camera(0, 30, 60);

	// Create the Floor
	Vertex3D a(-128, 0, -128);	
	Vertex3D b(-128, 0, 128);
	Vertex3D c(128, 0, 128);
	Vertex3D d(128, 0, -128);
	theFloor = Surface3D(a, b, c, d);
	theFloor.setColor(.1, .3, .1);
	Texture grass("grass.bmp", 512, 512);
	theFloor.setTexture(grass);

	// Create a light source and switch it on:
	// Set params:
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat position[] = { -1.5f, 1.0f, -4.0f, 1.0f };
	// Assign to GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	// Switch on GL_LIGHT0 and GL_LIGHTING
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Put some Fog
	float black[] = {.1, .1, .3, 0.7};
	glFogfv(GL_FOG_COLOR, black);
	glFogf(GL_FOG_START, 3.5);
	glFogf(GL_FOG_END, 5);
	glFogi(GL_FOG_MODE, GL_EXP2);
	//glEnable(GL_FOG);
    
}

/**** Methods for time measureing ****/

float theTime = 0;

#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/times.h>

#ifndef CLK_TCK
#define CLK_TCK 1
#endif

float timedelta(){

/*    static long begin = 0;
    static long finish, difference;

    static struct tms tb;
    finish = times(&tb);

    difference = finish - begin;
    begin = finish;

	return (float)difference/(float)CLK_TCK; */

    return .6; // for now a good value
}

void time(void){
	float dt = timedelta();
	theTime += dt;

	int SLOWDOWN = 20;

	if( (int)theTime % SLOWDOWN == 0){
		
		// TODO perform calculations on all WorldObjects
			

        	myRope.timeStep(dt);
		myCloth.timeStep(dt);
		myJelly.timeStep(dt);

	}

	glutPostRedisplay();

}

/**** OpenGL Display and Reshape ****/

void display(void){
	// Init frame:
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Update camera:
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 6, 500);
	Vector3D v = myCamera.getViewingDirection();
	Vector3D p = myCamera.getPosition();
	gluLookAt (p.x,p.y,p.z,   v.x,v.y,v.z,   0,1,0);
	
	// iterate over all "objects" and paint them (TODO list of worldobjects)

	theFloor.draw();
	myRope.draw();
	myJelly.draw();
	myCloth.draw();

	// Don't forget to swap the buffers...
	glDisable(GL_DEPTH_TEST);
	glutSwapBuffers();
}

void reshape(int width, int height){
   	 glViewport(0, 0, width, height);
}

void visible(int vis) {
	if (vis == GLUT_VISIBLE){
		glutIdleFunc(time);
	} else {
		glutIdleFunc(NULL);
	}
}

/**** OpenGL Mouse controls: ****/

void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    moving = 1;
    startx = x;
    starty = y;
  }
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    moving = 0;
  }
}

void motion(int x, int y) {
	if (moving) {
		//angle = angle + (x - startx);

		myCamera.turn_horizontal( x-startx );
		myCamera.turn_vertical( y-starty );

		//angle2 = angle2 + (y - starty);
		startx = x;
		starty = y;
		glutPostRedisplay();
	}
}

/**** OpenGL Keyboard controls: ****/

void keyboardSpecial(int key, int x, int y) {
	if( key == GLUT_KEY_LEFT){
		//myCamera.XCoord++;
		myCamera.move_left();
	}
	if( key == GLUT_KEY_RIGHT){
		//myCamera.XCoord--;
		myCamera.move_right();
	}
	if( key == GLUT_KEY_UP){
		myCamera.move_forward();
	}
	if( key == GLUT_KEY_DOWN){
		myCamera.move_back();
	}
	if( key == GLUT_KEY_PAGE_DOWN){
		myCamera.move_down();
	}
	if( key == GLUT_KEY_PAGE_UP){
		myCamera.move_up();
	}
}

void keyboard(unsigned char key, int x, int y) {

	/*if (key == 32){
		Force gravity(0, -1, 0);
		MassPoint3D* start = new MassPoint3D(0, 0, 0);
		myRope = Rope(start);
		myRope.applyGlobalForce(&gravity);
	}*/

	if (key == 27){
		exit(0);
	}

	glutPostRedisplay();
}



/**** OpenGL init and run ****/

void openGLrun(){
	glutMainLoop();
}

int openGLinit(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (600, 100);
	glutCreateWindow (argv[0]);

	init ();
	//create();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutVisibilityFunc(visible);

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
}

/**** Create Objects and run! ****/

int main(int argc, char** argv)
{
	
	// Init & Create the 3D world:
	openGLinit(argc, argv);
	
	// Add initial objects and forces to our world:
	
	Force gravity(0, -1, 0);

	MassPoint3D* start = new MassPoint3D(0, 50, 0);
	myRope = Rope(start);
	myRope.applyGlobalForce(&gravity);



	//Force smallgravity(0, -0.1, 0);

	MassPoint3D* start2 = new MassPoint3D(10, 30, 10);
	myCloth = Cloth(start2);
	myCloth.applyGlobalForce(&gravity);

    myJelly = Jelly();
    myJelly.applyGlobalForce(&gravity);
	// Start and show the 3D world:
	openGLrun();
	
	return 0;
}


