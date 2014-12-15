#version 400

in vec4 vertexPos;

out float posX, posY;

void main() {
	
	posX = vertexPos.x;
	posY = vertexPos.y;
	gl_Position = vertexPos;
	
}