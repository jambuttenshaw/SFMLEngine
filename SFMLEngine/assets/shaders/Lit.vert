#version 120

varying vec3 v_FragPos;

void main()
{
	// pass on the world pos of the fragment
	v_FragPos = gl_Vertex.xyz;

	// get the screen pos of the vertex
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	// assign the texture coordinates and the vertex colour
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
}
