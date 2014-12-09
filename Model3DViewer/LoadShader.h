#pragma once

#include <GL/glew.h>
#include <string>

int checkErrors(const std::string section);

GLuint initialiseShader(const char* vertShader, const char* fragShader);
GLuint compileShader(const char* vertShader, const char* fragShader);