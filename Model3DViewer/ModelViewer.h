#pragma once

#include "Mesh.h"
#include <vector>

class ModelViewer
{
public:
	// @argc and @argv are the command line parameters (they will be passed to GLUT)
	ModelViewer(char* name, int* argc, char* argv[]);
	~ModelViewer();
	void start();
	static std::vector<Mesh> _meshes;
private:
	static void display();
	static void reshape(int width, int height);
	static void keyboard(unsigned char key, int x, int y);
	static void special(int key, int x, int y);
	static void motion(int x, int y);
	static void mouse(int button, int state, int x, int y);
};

