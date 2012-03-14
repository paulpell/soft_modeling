
#include "World3D.h"

World3D::World3D(Camera c)
{
	camera = c;
}

World3D::~World3D() {
	// TODO Auto-generated destructor stub
}


void World3D::init(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize (400, 400);
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);

	//glutDisplayFunc(this->display);
	//glutReshapeFunc(reshape);

	//glutKeyboardFunc(camera.keyboard);
}

void World3D::createObjects()
{
	Vertex3D center(0,0,0);
	aCube = Cube3D(center, 1);
}

void World3D::reshape(int w, int h)
{

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glOrtho(-200.0, 200.0, -200.0, 200.0, -1.0, 60.0);
	glFrustum (-1, 1, -1, 1, 1.5, 20);

}

void World3D::display(void)
{
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt ( 2,2,3 , 0,0,0, 0,1,0 );

	aCube.draw();

	glutSwapBuffers();
}

void World3D::run()
{
	glutMainLoop();
}
