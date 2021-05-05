#version 120

uniform sampler2D texture;

uniform sampler2D u_TextureMask;

void main()
{

	// lookup the pixel in the main texture
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	// get the corresponding pixel in the mask texture
	vec4 mask = texture2D(u_TextureMask, gl_TexCoord[0].xy);

	// set the fragment color to the pixel multiplied by the mask
	gl_FragColor = mask * pixel;
}
