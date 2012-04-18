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


//#include "WorldObject.h"
#include "Rope.h"

Camera myCamera;

Surface3D theFloor;
Cube3D aCube;
Rope myRope;
Rope myRope2;

Spring mySpring; 

bool isOrtho;

GLfloat angle = 0;
GLfloat angle2 = 0; 

int moving, startx, starty;

void create()
{
	//Texture texture2("cat.bmp", 128, 128);
	Vertex3D center(0,0,0);
	aCube = Cube3D(center, 40);
	//aCube.addTexture(texture2, 1);
}

void init(void)
{
	isOrtho = false;

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	myCamera = Camera();
	myCamera.XCoord = 0;
	myCamera.YCoord = 30;	
	myCamera.ZCoord = 60;

	// Floor
	Vertex3D a(-128, 0, -128);	
	Vertex3D b(-128, 0, 128);
	Vertex3D c(128, 0, 128);
	Vertex3D d(128, 0, -128);
	theFloor = Surface3D(a, b, c, d);
	theFloor.setColor(.1, .3, .1);
	Texture grass("grass.bmp", 512, 512);
	theFloor.setTexture(grass);

	// Fog
	float black[] = {.1, .1, .3, 0};
	glFogfv(GL_FOG_COLOR, black);
	glFogf(GL_FOG_START, 3.5);
	glFogf(GL_FOG_END, 5);
	//glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);

	/*GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 0.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);*/

	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat position[] = { -1.5f, 1.0f, -4.0f, 1.0f };


	// Assign created components to GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glDisable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glDisable(GL_LIGHT0);
    
    glEnable(GL_POINT_SMOOTH);
    
}


void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (isOrtho){
		glOrtho(-15, 15, -15, 15, -2.0, 500);
	}else{
		//glFrustum (-10, 10, -10, 10, 6, 500);
		gluPerspective(60, 1, 6, 500);
	}

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt ( myCamera.XCoord,myCamera.YCoord,myCamera.ZCoord , 0,35,0, 0,1,0 );

	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	//aCube.draw();
	theFloor.draw();
	
	// TODO iterate over all "objects" and paint them
	
	//mySpring.draw();

	myRope.draw();
	//myRope2.draw();

	glDisable(GL_DEPTH_TEST);

	glutSwapBuffers();
}

void reshape(int width, int height)
{
   	 glViewport(0, 0, width, height);
}




void
mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    moving = 1;
    startx = x;
    starty = y;
  }
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    moving = 0;
  }
}


void
motion(int x, int y)
{
  if (moving) {
    angle = angle + (x - startx);
    angle2 = angle2 + (y - starty);
    startx = x;
    starty = y;
    glutPostRedisplay();
  }
}

void keyboard(unsigned char key, int x, int y)
{
	//Keyboard aKeyboard;
	//aKeyboard.processInput(key,x,y);

	// moving sideways on x
	if (key == 'q'){
		myCamera.XCoord += 1;
	}
	if (key == 'w'){
		myCamera.XCoord += 1;
	}
	if (key == 'e'){
		myCamera.XCoord -= 1;
	}
	

	if (key == 'a'){
		myCamera.ZCoord += 1;
	}
	if (key == 's'){
		myCamera.XCoord -= 1;
	}
	if (key == 'd'){
		myCamera.ZCoord -= 1;
	}


	if (key == 'z'){
		myCamera.YCoord += 1;
	}
	if (key == 'x'){
		//myCamera.currentZangle += 10;
	}
	if (key == 'c'){
		myCamera.YCoord -= 1;
	}

	if (key == 32){
		Force gravity(0, -1, 0);
		MassPoint3D* start = new MassPoint3D(0, 0, 0);
		myRope = Rope(start);
		myRope.applyGlobalForce(&gravity);
	}


	if (key == 27){
		exit(0);
	}

	if (key == 'p'){
		isOrtho = !isOrtho;
	}

	display();
}



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
//std::cout << difference << std::endl;
    begin = finish;

	return (float)difference/(float)CLK_TCK; */

    return .6; // for now a good value
}

void time(void){
	//std::cout << "time passes... " << std::endl;
	float dt = timedelta();
	theTime += dt;

	if( (int)theTime % 10 == 0){
		
		// TODO perform calculations on all WorldObjects
			

        	myRope.timeStep(dt);

	}

	glutPostRedisplay();

}

void visible(int vis) {
	if (vis == GLUT_VISIBLE){
		std::cout << "activate time... " << std::endl;
		glutIdleFunc(time);
	} else {
		glutIdleFunc(NULL);
	}
}

void openGLrun(){
	glutMainLoop();
}

int openGLinit(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);

	init ();
	create();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutVisibilityFunc(visible);

	glutKeyboardFunc(keyboard);
}



int main(int argc, char** argv)
{
	
	// Init & Create the 3D world:
	openGLinit(argc, argv);
	
	// Add initial objects and forces to our world:
	
	Force gravity(0, -1, 0);

	MassPoint3D* start = new MassPoint3D(0, 50, 0);
	myRope = Rope(start);
	myRope.applyGlobalForce(&gravity);

	// Start and show the 3D world:
	openGLrun();
	
	return 0;
}


