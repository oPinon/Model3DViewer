#version 400

uniform double ratio, zoom, offX, offY;

in float posX, posY;

out vec4 color;

void main() {

	// TODO : color_ramps

	double x0 = zoom * posX * ratio + offX;;
	double y0 = zoom * posY + offY;

	double x = x0;
	double y = y0;

	double xtemp = 0;
	double ytemp = 0;

	int iterations = 0;

	for(int i=0; i<200; i++) {

		if(x*x + y*y > 4.) { break; }
		else { iterations++; }

		xtemp = x*x - y*y + x0;
		ytemp = 2*x*y + y0;

		x = xtemp; y = ytemp;
	}

	float value = iterations / 200.;

	color = vec4( value, value, value, 1.);
}