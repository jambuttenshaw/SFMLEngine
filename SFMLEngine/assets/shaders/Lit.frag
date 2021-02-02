uniform sampler2D texture;

uniform sampler2D u_NormalMap;
uniform vec3 u_LightPos;

varying vec3 v_FragPos;

void main()
{

	// lookup the pixel in the main texture
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	
	// get the normal of the fragment
	vec3 normal = texture2D(u_NormalMap, gl_TexCoord[0].xy).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	// get the direction from the fragment to the light
	vec3 lightDir = normalize(u_LightPos - v_FragPos);
	lightDir.y = -lightDir.y;

	// find the dot product of the light direction and normal
	// which will tell us how lit this fragment is
	float lighting = dot(lightDir, normal);
	lighting = 0.5 * (lighting + 1.0);

	// assign the fragment colour as the sample from the texture multiplied by the lighting value
	gl_FragColor = vec4(lighting * pixel.rgb, pixel.a);
}
