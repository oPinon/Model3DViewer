#include "Mesh.h"

#include <fstream>
#include <iostream>

using namespace std;

Mesh::Mesh()
{
	setShader(0);
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

void Mesh::setShader(GLuint shaderID) {

	_shader.shaderID = shaderID;
	_shader.vertPos = glGetAttribLocation(shaderID, _shader.vertName);
	_shader.normalPos = glGetAttribLocation(shaderID, _shader.normalName);
	_shader.tanPos = glGetAttribLocation(shaderID, _shader.tanName);
	_shader.bitanPos = glGetAttribLocation(shaderID, _shader.bitanName);
}

void Mesh::render() {
	glPushMatrix(); {

		float scale = 0.5;
		glScalef(scale, scale, scale);
		glTranslatef(0.0, -4.0, 0.0);

		for (unsigned int i = 0; i < _faces.size(); i++) {
			Face f = _faces[i];
			glBegin(f.isQuad ? GL_QUADS : GL_TRIANGLES);

			if (f.hasTexCoords) glTexCoord2fv(&_texCoords[2 * f.vt0]);
			if (f.hasNormals) glNormal3fv(&_normals[3 * f.vn0]);
			//glVertex3fv(&_vertices[3 * f.v0]);
			glVertexAttrib3fv(0, &_vertices[3 * f.v0]);

			if (f.hasTexCoords) glTexCoord2fv(&_texCoords[2 * f.vt1]);
			if (f.hasNormals) glNormal3fv(&_normals[3 * f.vn1]);
			//glVertex3fv(&_vertices[3 * f.v1]);
			glVertexAttrib3fv(0, &_vertices[3 * f.v1]);

			if (f.hasTexCoords) glTexCoord2fv(&_texCoords[2 * f.vt2]);
			if (f.hasNormals) glNormal3fv(&_normals[3 * f.vn2]);
			//glVertex3fv(&_vertices[3 * f.v2]);
			glVertexAttrib3fv(0, &_vertices[3 * f.v2]);

			if (f.isQuad) {
				if (f.hasTexCoords) glTexCoord2fv(&_texCoords[2 * f.vt3]);
				if (f.hasNormals) glNormal3fv(&_normals[3 * f.vn3]);
				//glVertex3fv(&_vertices[3 * f.v3]);
				glVertexAttrib3fv(0, &_vertices[3 * f.v3]);
			}
			if (f.hasNormals) {
				glVertexAttrib3f(_shader.tanPos, f.tan.x, f.tan.y, f.tan.z);
				glVertexAttrib3f(_shader.bitanPos, f.bitan.x, f.bitan.y, f.bitan.z);
			}
			glEnd();
		}
	}
	glPopMatrix();
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

	FaceVertData v0 = getFaceVertData(line);
	toReturn.v0 = v0.v;
	toReturn.hasTexCoords = v0.hasVt;
	if (v0.hasVt) { toReturn.vt0 = v0.vt; }
	toReturn.hasNormals = v0.hasVn;
	if (v0.hasVn) { toReturn.vn0 = v0.vn; }

	line = line.substr(line.find(' ') + 1, string::npos);
	FaceVertData v1 = getFaceVertData(line);
	toReturn.v1 = v1.v;
	if (v1.hasVt) { toReturn.vt1 = v1.vt; }
	if (v1.hasVn) { toReturn.vn1 = v1.vn; }

	line = line.substr(line.find(' ') + 1, string::npos);
	FaceVertData v2 = getFaceVertData(line);
	toReturn.v2 = v2.v;
	if (v2.hasVt) { toReturn.vt2 = v2.vt; }
	if (v2.hasVn) { toReturn.vn2 = v2.vn; }

	line = line.substr(line.find(' ') + 1, string::npos);
	if (line.size() == 0) {
		toReturn.isQuad = false;
	}
	else {
		toReturn.isQuad = true;
		FaceVertData v3 = getFaceVertData(line);
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
			Normal normal = (normals[i])[j];
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

void Mesh::computeTangents() {

	struct Vec2 { float x, y; };

	for (Face f : _faces) {
		if (f.hasTexCoords) {

			Vec2 vt0 = { _texCoords[2 * f.vt0], _texCoords[2 * f.vt0 + 1] };
			Vec2 vt1 = { _texCoords[2 * f.vt1], _texCoords[2 * f.vt1 + 1] };
			Vec2 vt2 = { _texCoords[2 * f.vt2], _texCoords[2 * f.vt2 + 1] };
			Vec2 dt0 = { vt1.x - vt0.x, vt1.y - vt0.y };
			Vec2 dt1 = { vt2.x - vt0.x, vt2.y - vt0.y };
			float r = 1.0f / (dt0.x*dt1.y - dt0.y*dt1.x);

			Vec3 v0 = { _vertices[3 * f.v0 + 0], _vertices[3 * f.v0 + 1], _vertices[3 * f.v0 + 2] };
			Vec3 v1 = { _vertices[3 * f.v1 + 0], _vertices[3 * f.v1 + 1], _vertices[3 * f.v1 + 2] };
			Vec3 v2 = { _vertices[3 * f.v2 + 0], _vertices[3 * f.v2 + 1], _vertices[3 * f.v2 + 2] };
			Vec3 dv0 = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };
			Vec3 dv1 = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };

			f.tan = { r*(dv0.x*dt1.y - dv1.x*dt0.y), r*(dv0.y*dt1.y - dv1.y*dt0.y), r*(dv0.z*dt1.y - dv1.z*dt0.y) };
			f.bitan = { -r*(dv0.x*dt1.x - dv1.x*dt0.x), -r*(dv0.y*dt1.x - dv1.y*dt0.x), -r*(dv0.z*dt1.x - dv1.z*dt0.x) };
		}
	}
}