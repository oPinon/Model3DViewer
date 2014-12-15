#pragma once

#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>

class FractalViewer
{
public:
	FractalViewer(char* name, int* argc, char* argv[]);
	~FractalViewer();
	void start();
private:
	static void loadShader();
	static GLuint shaderID, ratioPos, zoomPos, offXPos, offYPos;
	static float zoom, offX, offY;
	static int _windowID, _width, _height;
	static void display();
	static void reshape(int width, int height);
	static void keyboard(unsigned char key, int x, int y);
	static void special(int key, int x, int y);
	static void motion(int x, int y);
	static void mouse(int button, int state, int x, int y);
};