varying vec3 ecPosition;
varying vec3 eyeVec;
varying vec3 normal;
varying vec3 tanVec;
varying vec3 bitanVec;
attribute vec3 vertexPosition;
attribute vec3 tangent;
attribute vec3 bitangent;

void main( void )
{

	vec4 vPosition = vec4( vertexPosition, 1.0);

   // transformation standard du sommet (ModelView et Projection)
   gl_Position = gl_ModelViewProjectionMatrix * vPosition;

   ecPosition = vec3( gl_ModelViewMatrix * vPosition );

   normal = normalize(gl_NormalMatrix * gl_Normal);

   eyeVec = -ecPosition;

   gl_TexCoord[0] = gl_MultiTexCoord0;

   tanVec = normalize( tangent );
   bitanVec = normalize( bitangent );
}