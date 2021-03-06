#include "ModelViewer.h"

#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>

int _windowID;
int _width = 800, _height = 800;
float _sensitivityX = 1.0, _sensitivityY = 1.0;
float _theta, _phi = -50;
std::vector<Mesh> ModelViewer::_meshes;

ModelViewer::ModelViewer(char* name, int* argc, char* argv[])
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
	glClearColor(0.1, 0.1, 0.1, 1.0);

	glEnable(GL_DEPTH_TEST);

}

void ModelViewer::start() {
	glutMainLoop();
}

void ModelViewer::display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// camera matrices
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)_width / (GLdouble)_height,
		0.1, 60.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(-3, 0, 0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
	glRotatef(_theta, 0.0, 0.0, 1.0);
	glRotatef(_phi, 0.0, 1.0, 0.0);

	for (auto& mesh : _meshes) {
		mesh.render();
	}

	glutSwapBuffers();

}

void ModelViewer::reshape(int width, int height) {

	_width = width; _height = height;
	glViewport(0, 0, _width, _height);
	glutPostRedisplay();
}

void ModelViewer::keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // escape
		glutDestroyWindow(_windowID);
		exit(0);
		break;
	case 'v' :
		for (auto& m : _meshes) { m.loadShader(); } break;
	}

	glutPostRedisplay();
}

void ModelViewer::special(int key, int x, int y) {

}

int _lastMouseX, _lastMouseY;

void ModelViewer::mouse(int button, int state, int x, int y) {

	if (state == GLUT_DOWN) {
		_lastMouseX = x;
		_lastMouseY = y;
	}
}

void ModelViewer::motion(int x, int y) {

	int dx = x - _lastMouseX;
	int dy = y - _lastMouseY;

	_lastMouseX = x;
	_lastMouseY = y;

	_phi += _sensitivityX * dx;
	_theta += _sensitivityY * dy;

	_phi = fmodf(_phi, 360);
	_theta = fmodf(_theta, 360);
	
	glutPostRedisplay();
}


ModelViewer::~ModelViewer()
{
}
