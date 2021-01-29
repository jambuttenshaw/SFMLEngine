uniform float u_DepthValue;

void main()
{
	vec4 vertex = gl_Vertex + vec4(0, 0, u_DepthValue, 0);

	gl_Position = gl_ModelViewProjectionMatrix * vertex;

	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
}
