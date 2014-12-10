uniform sampler2D textures[6];

varying vec3 position;
varying vec3 normal;
varying vec3 tanVec;
varying vec3 bitanVec;

void main( void )
{

	vec4 color = vec4(0.,0.,0.,1.);

	vec4 diffuse = texture2D( textures[0],  gl_TexCoord[0].st );
	vec4 locNormal = texture2D( textures[1],  gl_TexCoord[0].st );
	vec4 specular = texture2D( textures[2],  gl_TexCoord[0].st );
	vec4 alpha = texture2D( textures[3],  gl_TexCoord[0].st );
	vec4 emit = texture2D( textures[4],  gl_TexCoord[0].st );

	// Alpha
	if(alpha.x < 0.5) { discard; }

	// Normal Mapping
	vec3 N = normalize( tanVec * locNormal.r + bitanVec * locNormal.g + normal * locNormal.b );


	// MatCap
	vec4 matcap = texture2D( textures[5],  vec2( N.x + 1.0, N.y + 1.0 )/2.0 );
	float fresnel = pow( (1.-N.z), 2.);
	color += matcap * fresnel * max(0.,dot(- normalize(position),normal));

	// Diffuse and Specular lighting
	vec3 lightPos = vec3(20.,0.,5.);
	vec3 L = normalize( lightPos - position );
	float NdotL = abs(dot( N, L ));
	vec3 E = normalize( - position );
	vec3 R = -reflect( L, N );
	float NdotHV = max( dot( R, E ), 0.0 );
	color += diffuse * NdotL; // diffuse
	color += specular * pow( NdotHV, 8. ); // specular (with exposant)

	// Emit
	color += emit;

	gl_FragColor = color;

}