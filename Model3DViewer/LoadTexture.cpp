#include "LoadTexture.h"

#include <iostream>
#include <vector>

#include "bitmap.h"

bool loadTexture(std::string file, GLuint& texture) {

	std::cout << "loading texture " << file << " ... ";
	FILE* img;
	fopen_s(&img, file.c_str(), "r");
	if (!img) {
		std::cerr << "Missing texture file : " << file << std::endl;
		return false;
	}
	fclose(img);

	CBitmap bitmap(file.c_str());
	unsigned int size = bitmap.GetWidth() * bitmap.GetHeight() * 4;
	unsigned char* pixels = new unsigned char[size];
	bitmap.GetBits((void*)pixels, size, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, bitmap.GetWidth(), bitmap.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	delete[] pixels;

	std::cout << "done" << std::endl;

	return true;
}