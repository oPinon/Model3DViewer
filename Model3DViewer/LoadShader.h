#pragma once

#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <iostream>

int checkErrors(const std::string section);

GLuint initialiseShader(const char* vertShader, const char* fragShader);
GLuint compileShader(const char* vertShader, const char* fragShader);