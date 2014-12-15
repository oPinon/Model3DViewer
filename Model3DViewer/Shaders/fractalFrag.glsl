#version 140

in float posX, posY;

out vec4 color;

void main() {

	// TODO : color_ramps

	float x = posX;
	float y = posY;

	float xtemp = 0;
	float ytemp = 0;

	int iterations = 0;

	for(int i=0; i<200; i++) {

		if(x*x + y*y > 4.) { break; }
		else { iterations++; }

		xtemp = x*x - y*y + posX;
		ytemp = 2*x*y + posY;

		x = xtemp; y = ytemp;
	}

	float value = 1. - iterations / 200.;

	color = vec4( value, value, value, 1.);
}