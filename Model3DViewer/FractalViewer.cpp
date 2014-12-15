#include "FractalViewer.h"

#include <iostream>

#include "LoadShader.h"
#include "LoadTexture.h"

int FractalViewer::_windowID;
int FractalViewer::_width = 800, FractalViewer::_height = 800;

GLuint FractalViewer::shaderID;

FractalViewer::FractalViewer(char* name, int* argc, char* argv[])
{

	// GLUT initialisation

	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(_width, _height);

	_windowID = glutCreateWindow(name);

	glutDisplayFunc(&display);
	glutReshapeFunc(&reshape);
	glutKeyboardFunc(&keyboard);
	glutSpecialFunc(&special);
	glutMotionFunc(&motion);
	glutMouseFunc(&mouse);

	// GLEW initialisation
	glewInit();

	// OpenGL initialisation
	glClearColor(0.1, 0.9, 0.1, 1.0); // green is for debug purpose (it mustn't be seen)

	loadShader();

}

void FractalViewer::loadShader() {

	shaderID = initialiseShader("Shaders/fractalVert.glsl", "Shaders/FractalFrag.glsl");

}

void FractalViewer::start() {

	glutMainLoop();
}

void FractalViewer::display() {

	glClear(GL_COLOR_BUFFER_BIT);

	// Cameras
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0., 1., 1., 0., -1., 1.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Quad with fractals on it
	glUseProgram(shaderID);
	glBegin(GL_QUADS);
	glVertex2f(-1., -1.);
	glVertex2f(1., -1.);
	glVertex2f(1., 1.);
	glVertex2f(-1., 1.);
	glEnd();
	glUseProgram(0);

	glutSwapBuffers();
}

void FractalViewer::reshape(int width, int height) {

	_width = width; _height = height;
	glViewport(0, 0, _width, _height);
	glutPostRedisplay();
}

void FractalViewer::keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case 27: // escape
		glutDestroyWindow(_windowID);
		exit(0);
		break;
	case 'v':
		loadShader(); break;
	}

	glutPostRedisplay();
}

void FractalViewer::special(int key, int x, int y) {

}

void FractalViewer::motion(int x, int y) {

}

void FractalViewer::mouse(int button, int state, int x, int y) {

}

FractalViewer::~FractalViewer()
{

}