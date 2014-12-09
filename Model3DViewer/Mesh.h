#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include "LoadShader.h"

/*
* Loads a 3D model from a wavefront .obj
* Currently this only supports one texCoord and normal per vertex
* (in other words, different faces share the same values for normals and textures)
*/

struct Vec3 { float x, y, z; };

struct Face {
	bool isQuad; // else is triangle
	bool hasNormals;
	bool hasTexCoords;

	unsigned int v0, vt0, vn0;
	unsigned int v1, vt1, vn1;
	unsigned int v2, vt2, vn2;
	unsigned int v3, vt3, vn3;

	Vec3 tan, bitan;
};

struct ShaderAttribs {
	// shader program ID
	GLuint shaderID;
	// GLSL attributes' locations in the shader code
	GLint vertPos; char* vertName = "vertex";
	GLint normalPos; char* normalName = "normal";
	GLint tanPos; char* tanName = "tangent";
	GLint bitanPos; char* bitanName = "bitangent";
};

class Mesh
{
public:
	Mesh();
	~Mesh();
	// loads the model from a given .obj file
	void load(std::string filename);
	void render();
	void reloadShader();
	void setShader(const char* vertShader, const char* fragShader);

	std::vector<float> _vertices;
	std::vector<float> _texCoords;
	std::vector<unsigned int> _triangles;
	std::vector<unsigned int> _quads;
	std::vector<float> _normals; // in obj each lines do not correspond to vertices
	std::vector<Face> _faces;

private:
	ShaderAttribs _shader;
	void loadVertex(std::string& line);
	void loadFace(std::string& line);
	void loadTexCoords(std::string& line);
	void loadVertNormals(std::string& line);
	void computeNormals();
	void computeTangents();
};

