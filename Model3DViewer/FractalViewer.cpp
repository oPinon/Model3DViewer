#include "FractalViewer.h"

#include <iostream>

#include "LoadShader.h"
#include "LoadTexture.h"

int FractalViewer::_windowID;
int FractalViewer::_width = 800, FractalViewer::_height = 800;

GLuint FractalViewer::shaderID, FractalViewer::_ramp = 0;
GLuint FractalViewer::ratioPos, FractalViewer::zoomPos, FractalViewer::offXPos, FractalViewer::offYPos, FractalViewer::rampPos;
double FractalViewer::zoom = 1;
double FractalViewer::offX = 0, FractalViewer::offY = 0;

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

	loadTexture("Textures/colRamp.bmp", _ramp);

}

void FractalViewer::loadShader() {

	shaderID = initialiseShader("Shaders/fractalVert.glsl", "Shaders/FractalFrag.glsl");
	ratioPos = glGetUniformLocation(shaderID, "ratio");
	zoomPos = glGetUniformLocation(shaderID, "zoom");
	offXPos = glGetUniformLocation(shaderID, "offX");
	offYPos = glGetUniformLocation(shaderID, "offY");
	rampPos = glGetUniformLocation(shaderID, "colRamp");
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

	glUseProgram(shaderID);

	// sending parameters to shader
	glUniform1d(ratioPos, ((double)_width) / _height);
	glUniform1d(zoomPos, zoom);
	glUniform1d(offXPos, offX);
	glUniform1d(offYPos, offY);
	glUniform1i(shaderID, _ramp);

	// Quad with fractals on it
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
	case 'v': // reload shader
		loadShader(); break;
	case 'r': // reset view
		zoom = 1; offX = 0; offY = 0;
		break;
	}

	glutPostRedisplay();
}

void FractalViewer::special(int key, int x, int y) {

}

void FractalViewer::motion(int x, int y) {

}

void FractalViewer::mouse(int button, int state, int x, int y) {

	if (state == GLUT_DOWN) {

		offX += zoom * 2*((float)x - _width / 2) / _width;
		offY -= zoom * 2*((float)y - _height / 2) / _height;

		switch (button) {
		case GLUT_LEFT_BUTTON :
			zoom /= 2.0;
			break;
		case GLUT_RIGHT_BUTTON:
			zoom *= 2.0;
			break;
		}

	}

	glutPostRedisplay();
}

FractalViewer::~FractalViewer()
{

}