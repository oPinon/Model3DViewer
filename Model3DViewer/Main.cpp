// standard libraries
#include <iostream>

// external libraries



// project imports
#include "ModelViewer.h"
#include "FractalViewer.h"
#include "Mesh.h"

int main(int argc, char *argv[]) {

	/*ModelViewer view("Model3D view", &argc, argv);

	Mesh mesh;
	mesh.load("DarkSiders.obj");
	view._meshes.push_back(mesh);*/

	FractalViewer view("Fractal view", &argc, argv);
	view.start();
}