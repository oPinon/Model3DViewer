#version 140

uniform float ratio, zoom, offX, offY;

in vec4 vertexPos;

out float posX, posY;

void main() {
	
	// TODO : compute screen ratio and zoom factors / translations

	gl_Position = vertexPos;
	posX = zoom * vertexPos.x * ratio + offX;
	posY = zoom * vertexPos.y + offY;
}