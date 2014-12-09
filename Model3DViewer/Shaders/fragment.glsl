varying vec3 ecPosition;
varying vec3 eyeVec;
varying vec3 normal;
varying vec3 tanVec;
varying vec3 bitanVec;

void main( void )
{

	vec3 lightDir = vec3(0.,0.5,0.5);

	gl_FragColor = vec4( 1.0, 1.0, 1.0, 1.0 ) * dot(normal, lightDir);

}