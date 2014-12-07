#pragma once

#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>

static class ModelViewer
{
public:
	// @argc and @argv are the command line parameters (they will be passed to GLUT)
	ModelViewer(char* name, int* argc, char* argv[]);
	~ModelViewer();
private:
	static void display();
	static void reshape(int width, int height);
	static void keyboard(unsigned char key, int x, int y);
	static void special(int key, int x, int y);
	static void motion(int x, int y);
	static void mouse(int button, int state, int x, int y);
};

