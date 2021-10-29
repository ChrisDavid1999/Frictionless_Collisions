#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <reactphysics3d/body/RigidBody.h>
#include <glm/glm.hpp>

#include "Classes/WorldManager.h"

float angle = 0.0f;
float lx = 0.0f, lz = -1.0f;
float x = 0.0f, z = 5.0f;
float deltaAngle = 0.0f;
int xOrigin = -1;

void changeSize(int w, int h) {

	if (h == 0)
		h = 1;

	float ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {

	Manager::getInstance().DetlaTimeUpdate(glutGet(GLUT_ELAPSED_TIME));
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	gluLookAt(x, 1.0f, z,
		x + lx, 1.0f, z + lz,
		0.0f, 1.0f, 0.0f);

	//Do other stuff here
	Manager::getInstance().Update();
	Manager::getInstance().RenderUpdate();
	
	glutSwapBuffers();
}

void keys(unsigned char key, int xx, int yy) {

	if (key == 27)
		exit(0);
}

void pressKey(int key, int xx, int yy) {
	
}

void releaseKey(int key, int x, int y) {

}

void mouseMove(int x, int y) {

	if (xOrigin >= 0) {

		deltaAngle = (x - xOrigin) * 0.001f;

		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
}

void mouseButton(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) {

		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else {
			xOrigin = x;
		}
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	Manager::getInstance().Init();
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("Lighthouse3D - GLUT Tutorial");


	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(keys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();

	return 1;
}