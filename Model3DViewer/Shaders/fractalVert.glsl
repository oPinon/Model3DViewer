#version 140

in vec4 vertexPos;

out float posX, posY;

void main() {
	
	// TODO : compute screen ratio and zoom factors / translations

	gl_Position = vertexPos;
	posX = vertexPos.x;
	posY = vertexPos.y;
}