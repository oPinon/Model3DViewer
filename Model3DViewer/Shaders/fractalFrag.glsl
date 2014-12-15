#version 400

uniform double ratio, zoom, offX, offY;
uniform sampler2D colRamp;

in float posX, posY;

out vec4 color;

void main() {

	double x0 = zoom * posX * ratio + offX;;
	double y0 = zoom * posY + offY;

	double x = x0;
	double y = y0;

	double xtemp = 0;
	double ytemp = 0;

	int iterations = 0;
	int maxIterations = 256;

	for(int i=0; i< maxIterations; i++) {

		if(x*x + y*y > 4.) { break; }
		else { iterations++; }

		xtemp = x*x - y*y + x0;
		ytemp = 2*x*y + y0;

		x = xtemp; y = ytemp;
	}

	if( iterations == maxIterations ) {

		color = vec4( vec3(0.), 1. );
	}
	else {

		float value = ( iterations * 1.0 ) / maxIterations;
		color = texture2D( colRamp, vec2( value, 0. ));
	}
}