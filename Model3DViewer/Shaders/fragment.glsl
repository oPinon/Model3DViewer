uniform sampler2D textures[6];

varying vec3 ecPosition;
varying vec3 eyeVec;
varying vec3 normal;
varying vec3 tanVec;
varying vec3 bitanVec;

void main( void )
{

	vec3 lightDir = vec3(0.,0.5,0.5);

	vec4 color = texture2D( textures[0],  gl_TexCoord[0].st );

	//color += vec4(0.2,0.0,0.0,0.0);

	gl_FragColor = color * dot(normal, lightDir);

}