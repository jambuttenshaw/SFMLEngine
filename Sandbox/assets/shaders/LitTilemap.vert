uniform float u_DepthValue;

uniform vec2 u_PosOffset;

varying vec3 v_FragPos;

void main()
{
	// order the vertex's z position
	// used so that I can draw the sprites in any order
	// and then the depth test takes care of ordering them on the screen
	vec4 vertex = gl_Vertex + vec4(0, 0, u_DepthValue, 0);

	// pass on the world pos of the fragment
	v_FragPos = gl_Vertex + vec4(u_PosOffset, 0, 0);

	// get the screen pos of the vertex
	gl_Position = gl_ModelViewProjectionMatrix * vertex;

	// assign the texture coordinates and the vertex colour
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
}
