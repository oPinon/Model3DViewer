#include "LoadShader.h"

int checkErrors(const std::string section) {

	int count = 0;
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		const GLubyte* text = gluErrorString(err);
		std::cerr << "Error at : " << section.c_str() << std::endl << text << std::endl;
		++count;
	}
	return count;
}

static void displayShaderLog(GLuint shader, std::string message) {

	std::cout << message.c_str() << " : ";

	int infologLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1) {

		char* infolog = new char[infologLength + 1];
		int charsWritten = 0;
		glGetShaderInfoLog(shader, infologLength, &charsWritten, infolog);
		std::cout << infolog << std::endl;
		delete[] infolog;
	}
	else {
		std::cout << "No error" << std::endl;
	}
}

GLuint compileShader(const char* vertShader, const char* fragShader) {

	GLuint shaderID = glCreateProgram();

	// vertex shader
	GLuint vertShaderObj = glCreateShader(GL_VERTEX_SHADER);
	if (vertShader!= NULL) {
		glShaderSource(vertShaderObj, 1, &vertShader, NULL);
		glCompileShader(vertShaderObj);
		glAttachShader(shaderID, vertShaderObj);
		displayShaderLog(vertShaderObj, "Vertex Shader");
	}
	//else { return -1; }

	// fragment shader
	GLuint fragShaderObj = glCreateShader(GL_FRAGMENT_SHADER);
	if (fragShader != NULL) {
		glShaderSource(fragShaderObj, 1, &fragShader, NULL);
		glCompileShader(fragShaderObj);
		glAttachShader(shaderID, fragShaderObj);
		displayShaderLog(fragShaderObj, "Fragment Shader");
	}
	//else { return -1; }

	glLinkProgram(shaderID);
	displayShaderLog(shaderID, "Shader");
	return shaderID;
}

static char* readShader(const char* filename) {
	char* content = NULL;
	if (filename != NULL) {
		FILE* file;
		fopen_s( &file, filename, "r");
		if (file == NULL) {
			perror(filename);
		}
		else {
			fseek(file, 0, SEEK_END);
			int size = ftell(file);
			if (size > 0) {
				rewind(file);
				content = new char[size + 1];
				size = fread(content, sizeof(char), size, file);
				content[size] = '\0';
			}
			fclose(file);
		}
	}
	return content;
}

GLuint initialiseShader(const char* vertShader, const char* fragShader) {

	const char* vertShaderText = readShader(vertShader);
	const char* fragShaderText = readShader(fragShader);

	return compileShader(vertShaderText, fragShaderText);
}