//============================================================================
// Name        : TestCpp.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

/*
 * main.cpp
 *
 *  Created on: Feb 27, 2012
 *      Author: terix
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>

//include "Camera.h"
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
//#include "Rope.h"
//Rope myRope;



Camera myCamera;

Surface3D aSurface;
Cube3D aCube;

Spring mySpring; 


bool isOrtho;


void create()
{


	Texture texture2("cat.bmp", 128, 128);
	Vertex3D center(0,0,0);
	aCube = Cube3D(center, 25);
	aCube.addTexture(texture2, 1);


}

void init(void)
{

	isOrtho = false;

	glClearColor (0.0, 0.0, 0.0, 0.0);
	//glShadeModel (GL_FLAT);
	glShadeModel (GL_SMOOTH);

	myCamera = Camera();
	myCamera.XCoord = 5;
	myCamera.YCoord = 2;	
	myCamera.ZCoord = 6;
	/*myCamera.currentXangle = 0;
	myCamera.currentYangle = 0;
	myCamera.currentZangle = 30;*/


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

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
		//glDisable(GL_LIGHT0);
}


void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (isOrtho){
		glOrtho(-15, 15, -15, 15, -2.0, 500);
	}else{
		glFrustum (-10, 10, -10, 10, 6, 500);
	}

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt ( myCamera.XCoord,myCamera.YCoord,myCamera.ZCoord , 0,0,0, 0,1,0 );


	aCube.draw();

	
	// TODO iterate over all "objects" and paint them
	mySpring.draw();


	//glMatrixMode(GL_PROJECTION);	
	//glLoadIdentity();
	//myCamera.move_camera();

	
	glDisable(GL_DEPTH_TEST);

	glutSwapBuffers();
}

void reshape(int w, int h)
{

	glViewport (0, 0, 600, 600);


	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (isOrtho){
		glOrtho(-2, 2, -2, 2, -1.0, 10);
	}else{
		glFrustum (-1, 1, -1, 1, 2, 20);
	}*/

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
		//myCamera.rotate_xaxis(20);
		//return;
	}
	if (key == 'e'){
		myCamera.XCoord -= 1;
	}
	

	if (key == 'a'){
		myCamera.YCoord += 1;
	}
	if (key == 's'){
		//myCamera.rotate_yaxis(10);
	}
	if (key == 'd'){
		myCamera.YCoord -= 1;
	}


	if (key == 'z'){
		myCamera.ZCoord += 1;
	}
	if (key == 'x'){
		//myCamera.currentZangle += 10;
	}
	if (key == 'c'){
		myCamera.ZCoord -= 1;
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

float timedelta(){
	return 1;
}

void time(void){

	float dt = timedelta();
	theTime += dt;

	if( (int)theTime % 100 == 0){
		
		// TODO perform calculations on all WorldObjects	
		mySpring.timeStep( dt );
	
	}

	glutPostRedisplay();

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
	glutIdleFunc(time);

	glutKeyboardFunc(keyboard);
}







int main(int argc, char** argv)
{
	
	// Init & Create the 3D world:
	openGLinit(argc, argv);
	
	// Add initial objects and forces to our world:
	//Force gravity(0, -9.8, 0);
	Force gravity(0, -1, 0);
	MassPoint3D start(0, 0, 0);
	start.setAnchor(true);
	MassPoint3D end(5, 0, 0);
	end.addForce(&gravity);

	mySpring = Spring(&start, &end, 1);

	// Start and show the 3D world:
	openGLrun();

	//myRope = Rope(0, 0, 0,    5, 0, 0); 
	//myRope.applyGeneral(gravity)
	
	return 0;
}


