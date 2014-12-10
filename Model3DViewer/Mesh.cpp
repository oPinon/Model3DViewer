#include "Mesh.h"

#include <fstream>
#include <iostream>

#include "LoadShader.h"
#include "LoadTexture.h"

using namespace std;

void Mesh::loadShader() {
	setShader("Shaders/vertex.glsl", "Shaders/fragment.glsl");
}

void Mesh::loadTextures() {

	glActiveTexture(GL_TEXTURE0);
	loadTexture("Textures/Diffuse.bmp", _textures.diffuse);

	glActiveTexture(GL_TEXTURE1);
	loadTexture("Textures/Normals.bmp", _textures.normals);

	glActiveTexture(GL_TEXTURE2);
	loadTexture("Textures/Specular.bmp", _textures.spec);

	glActiveTexture(GL_TEXTURE3);
	loadTexture("Textures/Alpha.bmp", _textures.alpha);

	glActiveTexture(GL_TEXTURE4);
	loadTexture("Textures/Emit.bmp", _textures.emit);

	glActiveTexture(GL_TEXTURE5);
	loadTexture("Textures/MatCap.bmp", _textures.matCap);

}

Mesh::Mesh()
{
	loadShader();
	loadTextures();
}

Mesh::~Mesh()
{
}

void Mesh::load(std::string filename) {

	ifstream file;
	file.open(filename, ios::in, ios::binary);
	if (!file.is_open()) { cerr << "Error : couldn't open file " << filename << endl; return;  }

	cout << "loading : " << filename << " ...";

	string line;
	while (getline(file, line)) {

		if (line.length() < 1) { continue; }

		char tag = line[0];

		switch (tag)
		{
		case 'o':
			//_name = line.substr(2, string::npos);
			break;

		case 'f': // case of face
			loadFace(line.substr(2, string::npos));
			break;

		case 'v': // case of vertex data
			char second_tag = line[1];
			switch (second_tag)
			{
			case 't': // texture coordinates
				loadTexCoords(line.substr(3, string::npos));
				break;

			case ' ': // vertex coordinates
				loadVertex(line.substr(2, string::npos));
				break;

			case 'n': // vertex normal
				loadVertNormals(line.substr(3, string::npos));
				break;
			}
			break;
		}
	}
	cout << " done" << endl;
	cout << "vertices : " << _vertices.size() / 3 << endl;
	cout << "faces : " << _faces.size() << endl;

	//cout << "computing normals... "; computeNormals(); cout << "done" << endl;
	cout << "computing tangents... "; computeTangents(); cout << "done" << endl;

	file.close();
}

void Mesh::setShader(const char* vertShader, const char* fragShader) {

	GLuint shaderID = initialiseShader(vertShader, fragShader);

	_shader.shaderID = shaderID;
	_shader.texPos = glGetUniformLocation(shaderID, _shader.texName);
	_shader.vertPos = glGetAttribLocation(shaderID, _shader.vertName);
	_shader.normalPos = glGetAttribLocation(shaderID, _shader.normalName);
	_shader.tanPos = glGetAttribLocation(shaderID, _shader.tanName);
	_shader.bitanPos = glGetAttribLocation(shaderID, _shader.bitanName);

}

const GLint samplers[6] = { 0, 1, 2, 3, 4, 5 };

void Mesh::render() {

	glUseProgram(_shader.shaderID);

	glPushMatrix(); {

		float scale = 0.5;
		glScalef(scale, scale, scale);
		glTranslatef(0.0, -4.0, 0.0);

		glUniform1iv(_shader.texPos, 6, samplers);

		for (unsigned int i = 0; i < _faces.size(); i++) {
			Face& f = _faces[i];
			glBegin(f.isQuad ? GL_QUADS : GL_TRIANGLES);

			if (f.hasTexCoords) glTexCoord2fv(&_texCoords[2 * f.vt0]);
			if (f.hasNormals) {
				glNormal3fv(&_normals[3 * f.vn0]);
				glVertexAttrib3f(_shader.tanPos, _tangents[f.v0].x, _tangents[f.v0].y, _tangents[f.v0].z);
				glVertexAttrib3f(_shader.bitanPos, _bitangents[f.v0].x, _bitangents[f.v0].y, _bitangents[f.v0].z);
			}
			glVertexAttrib3fv(0, &_vertices[3 * f.v0]);

			if (f.hasTexCoords) glTexCoord2fv(&_texCoords[2 * f.vt1]);
			if (f.hasNormals) {
				glNormal3fv(&_normals[3 * f.vn1]);
				glVertexAttrib3f(_shader.tanPos, _tangents[f.v1].x, _tangents[f.v1].y, _tangents[f.v1].z);
				glVertexAttrib3f(_shader.bitanPos, _bitangents[f.v1].x, _bitangents[f.v1].y, _bitangents[f.v1].z);
			}
			glVertexAttrib3fv(0, &_vertices[3 * f.v1]);

			if (f.hasTexCoords) glTexCoord2fv(&_texCoords[2 * f.vt2]);
			if (f.hasNormals) {
				glNormal3fv(&_normals[3 * f.vn2]);
				glVertexAttrib3f(_shader.tanPos, _tangents[f.v2].x, _tangents[f.v2].y, _tangents[f.v2].z);
				glVertexAttrib3f(_shader.bitanPos, _bitangents[f.v2].x, _bitangents[f.v2].y, _bitangents[f.v2].z);
			}
			glVertexAttrib3fv(0, &_vertices[3 * f.v2]);

			if (f.isQuad) {
				if (f.hasTexCoords) glTexCoord2fv(&_texCoords[2 * f.vt3]);
				if (f.hasNormals) {
					glNormal3fv(&_normals[3 * f.vn3]);
					glVertexAttrib3f(_shader.tanPos, _tangents[f.v3].x, _tangents[f.v3].y, _tangents[f.v3].z);
					glVertexAttrib3f(_shader.bitanPos, _bitangents[f.v3].x, _bitangents[f.v3].y, _bitangents[f.v3].z);
				}
				glVertexAttrib3fv(0, &_vertices[3 * f.v3]);
			}
			glEnd();
		}
	}
	glPopMatrix();

	glUseProgram(0);
}

void  Mesh::loadVertex(std::string& line) {
	float x, y, z;
	int splitPos;

	splitPos = line.find(' ');
	x = stof(line.substr(0, splitPos));
	line = line.substr(splitPos + 1, string::npos);

	splitPos = line.find(' ');
	y = stof(line.substr(0, splitPos));
	line = line.substr(splitPos + 1, string::npos);

	z = std::stof(line);

	_vertices.push_back(x);
	_vertices.push_back(y);
	_vertices.push_back(z);
}

struct FaceVertData{
	unsigned int v;
	bool hasVt;
	unsigned int vt;
	bool hasVn;
	unsigned int vn;
};

FaceVertData getFaceVertData(string line) { // parse a line of the format int or int/int or int/int/int or int//int
	FaceVertData toReturn;
	toReturn.v = stoi(line) - 1;
	int slashPos;
	slashPos = line.find('/');
	if (slashPos == string::npos) { // int
		toReturn.hasVt = false; toReturn.hasVn = false;
		return toReturn;
	}
	else {
		line = line.substr(slashPos + 1, string::npos);
		slashPos = line.find('/');
		if (slashPos == 0) { // int//int
			toReturn.hasVt = false; toReturn.hasVn = true;
			line = line.substr(slashPos + 1, string::npos);
			toReturn.vn = stoi(line) - 1;
			return toReturn;
		}
		else { // int/int..
			toReturn.hasVt = true;
			toReturn.vt = stoi(line) - 1;
			if (slashPos == string::npos) { // int/int
				toReturn.hasVn = false;
				return toReturn;
			}
			else { // int/int/int
				toReturn.hasVn = true;
				line = line.substr(slashPos + 1, string::npos);
				toReturn.vn = stoi(line) - 1;
				return toReturn;
			}
		}
	}
};

void  Mesh::loadFace(std::string& line) {

	Face toReturn;

	FaceVertData& v0 = getFaceVertData(line);
	toReturn.v0 = v0.v;
	toReturn.hasTexCoords = v0.hasVt;
	if (v0.hasVt) { toReturn.vt0 = v0.vt; }
	toReturn.hasNormals = v0.hasVn;
	if (v0.hasVn) { toReturn.vn0 = v0.vn; }

	line = line.substr(line.find(' ') + 1, string::npos);
	FaceVertData& v1 = getFaceVertData(line);
	toReturn.v1 = v1.v;
	if (v1.hasVt) { toReturn.vt1 = v1.vt; }
	if (v1.hasVn) { toReturn.vn1 = v1.vn; }

	line = line.substr(line.find(' ') + 1, string::npos);
	FaceVertData& v2 = getFaceVertData(line);
	toReturn.v2 = v2.v;
	if (v2.hasVt) { toReturn.vt2 = v2.vt; }
	if (v2.hasVn) { toReturn.vn2 = v2.vn; }

	int spacePos = line.find(' ');
	if (spacePos == -1) {
		toReturn.isQuad = false;
	}
	else {
		line = line.substr(spacePos+1, string::npos);
		toReturn.isQuad = true;
		FaceVertData& v3 = getFaceVertData(line);
		toReturn.v3 = v3.v;
		if (v3.hasVt) { toReturn.vt3 = v3.vt; }
		if (v3.hasVn) { toReturn.vn3 = v3.vn; }
	}

	_faces.push_back(toReturn);
}

void  Mesh::loadTexCoords(std::string& line) {
	float tx, ty;
	int splitPos;

	splitPos = line.find(' ');
	tx = stof(line.substr(0, splitPos));
	line = line.substr(splitPos + 1, string::npos);

	ty = stof(line);

	_texCoords.push_back(tx);
	_texCoords.push_back(ty);
}

void  Mesh::loadVertNormals(std::string& line) {
	float nx, ny, nz;
	int splitPos;

	splitPos = line.find(' ');
	nx = stof(line.substr(0, splitPos));
	line = line.substr(splitPos + 1, string::npos);

	splitPos = line.find(' ');
	ny = stof(line.substr(0, splitPos));
	line = line.substr(splitPos + 1, string::npos);

	splitPos = line.find(' ');
	nz = stof(line.substr(0, splitPos));

	_normals.push_back(nx);
	_normals.push_back(ny);
	_normals.push_back(nz);
}

struct Normal { float x, y, z; };

void addNormal(unsigned int* triangle, vector<float>& _vertices, vector< vector<Normal> >& normals) {
	float v0x = _vertices[triangle[0] + 0], v0y = _vertices[triangle[0] + 1], v0z = _vertices[triangle[0] + 2];
	float v1x = _vertices[triangle[1] + 0], v1y = _vertices[triangle[1] + 1], v1z = _vertices[triangle[1] + 2];
	float v2x = _vertices[triangle[2] + 0], v2y = _vertices[triangle[2] + 1], v2z = _vertices[triangle[2] + 2];

	float d0x = v1x - v0x, d0y = v1y - v0y, d0z = v1z - v0z;
	float d1x = v2x - v0x, d1y = v2y - v0y, d1z = v2z - v0z;

	Normal normal;
	normal.x = (d0y*d1z - d1y*d0z);
	normal.y = (d0z*d1x - d1z*d0x);
	normal.z = (d0x*d1y - d1x*d0y);
	float norm = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
	if (norm == 0) { cerr << "ERROR : found 3 colinear vertices in a face : cannot compute normals" << endl; }
	else {
		normal.x /= norm;
		normal.y /= norm;
		normal.z /= norm;
	}
	normals[triangle[0]].push_back(normal);
	normals[triangle[1]].push_back(normal);
	normals[triangle[2]].push_back(normal);
};

void Mesh::computeNormals() {

	vector< vector<Normal> > normals(_vertices.size() / 3); // list of normals for each vertex

	for (unsigned int i = 0; i < _triangles.size(); i += 3) { // add to list from triangles
		addNormal(&_triangles[i], _vertices, normals);
	}
	for (unsigned int i = 0; i < _quads.size(); i += 4) { // add to list from quads
		addNormal(&_quads[i], _vertices, normals);
		addNormal(&_quads[i + 1], _vertices, normals);
	}

	_normals.resize(_vertices.size());

	for (unsigned int i = 0; i < _vertices.size() / 3; i++) { // average all normals for each vertex
		float nx = 0, ny = 0, nz = 0;
		for (unsigned int j = 0; j < normals[i].size(); j++) {
			Normal& normal = (normals[i])[j];
			nx += normal.x;
			ny += normal.y;
			nz += normal.z;
		}

		float norm = sqrt(nx*nx + ny*ny + nz*nz);
		if (norm != 0) {
			nx /= norm;
			ny /= norm;
			nz /= norm;
		}
		_normals[3 * i + 0] = nx;
		_normals[3 * i + 1] = ny;
		_normals[3 * i + 2] = nz;
	}
}

struct Vec2 { float x, y; };

void normalize(Vec3& v) {
	float norm = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	v.x /= norm; v.y /= norm; v.z /= norm;
}

void Mesh::computeTriangle(unsigned int& v0, unsigned int& v1, unsigned int& v2, unsigned int& vt0, unsigned int& vt1, unsigned int& vt2, vector<vector<Vec3>>& tanLists, vector<vector<Vec3>>& bitanLists) {

	// vertices
	Vec3 vert0 = { _vertices[3 * v0 + 0], _vertices[3 * v0 + 1], _vertices[3 * v0 + 2] };
	Vec3 vert1 = { _vertices[3 * v1 + 0], _vertices[3 * v1 + 1], _vertices[3 * v1 + 2] };
	Vec3 vert2 = { _vertices[3 * v2 + 0], _vertices[3 * v2 + 1], _vertices[3 * v2 + 2] };

	// texture coordinates
	Vec2 uv0 = { _texCoords[2 * vt0], _texCoords[2 * vt0 + 1] };
	Vec2 uv1 = { _texCoords[2 * vt1], _texCoords[2 * vt1 + 1] };
	Vec2 uv2 = { _texCoords[2 * vt2], _texCoords[2 * vt2 + 1] };

	// vertices deltas
	Vec3 deltaPos1 = { vert1.x - vert0.x, vert1.y - vert0.y, vert1.z - vert0.z };
	Vec3 deltaPos2 = { vert2.x - vert0.x, vert2.y - vert0.y, vert2.z - vert0.z };

	// texCoord deltas
	Vec2 deltaUV1 = { uv1.x - uv0.x, uv1.y - uv0.y };
	Vec2 deltaUV2 = { uv2.x - uv0.x, uv2.y - uv0.y };

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y*deltaUV2.x);

	Vec3 tan = { r*(deltaPos1.x*deltaUV2.y - deltaPos2.x*deltaUV1.y), r*(deltaPos1.y*deltaUV2.y - deltaPos2.y*deltaUV1.y), r*(deltaPos1.z*deltaUV2.y - deltaPos2.z*deltaUV1.y) };
	Vec3 bitan = { r*(deltaPos2.x*deltaUV1.x - deltaPos1.x*deltaUV2.x), r*(deltaPos2.y*deltaUV1.x - deltaPos1.y*deltaUV2.x), r*(deltaPos2.z*deltaUV1.x - deltaPos1.z*deltaUV2.x) };

	normalize(tan); normalize(bitan);

	tanLists[v0].push_back(tan);
	tanLists[v1].push_back(tan);
	tanLists[v2].push_back(tan);

	bitanLists[v0].push_back(bitan);
	bitanLists[v1].push_back(bitan);
	bitanLists[v2].push_back(bitan);
}

void average(const vector<vector<Vec3>>& src, vector<Vec3>& dst) {

	for (unsigned int i = 0; i < dst.size(); i++) {
		Vec3 sum = { 0, 0, 0 };
		for (auto& v : src[i]) {
			sum.x += v.x;
			sum.y += v.y;
			sum.z += v.z;
		}
		int size = src[i].size();
		if (size != 0) {
			dst[i] = { sum.x / size, sum.y / size, sum.z / size };
		}
	}
}

void Mesh::computeTangents() {

	unsigned int size = _vertices.size() / 3;
	vector<vector<Vec3>> tanLists(size), bitanLists(size);

	for (Face& f : _faces) {
		if (f.hasTexCoords) {

			computeTriangle(f.v0, f.v1, f.v2, f.vt0, f.vt1, f.vt2, tanLists, bitanLists);

			if (f.isQuad) {
				computeTriangle(f.v1, f.v2, f.v3, f.vt1, f.vt2, f.vt3, tanLists, bitanLists);
			}
		}
	}

	_tangents = vector<Vec3>(size);
	_bitangents = vector<Vec3>(size);

	average(tanLists, _tangents);
	average(bitanLists, _bitangents);

}