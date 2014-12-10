#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include <array>

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
	GLint texPos; char* texName = "textures";
	GLint vertPos; char* vertName = "vertex";
	GLint normalPos; char* normalName = "normal";
	GLint tanPos; char* tanName = "tangent";
	GLint bitanPos; char* bitanName = "bitangent";
};

struct Textures {
	GLuint diffuse;
	GLuint normals;
	GLuint spec;
	GLuint alpha;
	GLuint emit;
	GLuint matCap;
};

class Mesh
{
public:
	Mesh();
	~Mesh();
	// loads the model from a given .obj file
	void load(std::string filename);
	void render();
	void loadShader();
	void loadTextures();
	void setShader(const char* vertShader, const char* fragShader);

	std::vector<float> _vertices;
	std::vector<float> _texCoords;
	std::vector<unsigned int> _triangles;
	std::vector<unsigned int> _quads;
	std::vector<float> _normals; // in obj each lines does not correspond to a vertex
	std::vector<Face> _faces;
	std::vector<Vec3> _tangents, _bitangents;

private:
	ShaderAttribs _shader;
	Textures _textures;
	void loadVertex(std::string& line);
	void loadFace(std::string& line);
	void loadTexCoords(std::string& line);
	void loadVertNormals(std::string& line);
	void computeNormals();
	void computeTangents();
	void computeTriangle(unsigned int& v0, unsigned int& v1, unsigned int& v2, unsigned int& vt0, unsigned int& vt1, unsigned int& vt2, std::vector<std::vector<Vec3>>& tanLists, std::vector<std::vector<Vec3>>& bitanLists);
};

