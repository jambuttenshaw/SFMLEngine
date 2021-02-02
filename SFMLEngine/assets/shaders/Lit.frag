uniform sampler2D texture;

uniform sampler2D u_NormalMap;

uniform vec3 u_LightPos;
uniform float u_LightIntensity;
uniform float u_LightRange;
uniform vec4 u_LightColor;

varying vec3 v_FragPos;

void main()
{

	// lookup the pixel in the main texture
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	
	// get the normal of the fragment
	vec3 normal = texture2D(u_NormalMap, gl_TexCoord[0].xy).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	// get the direction from the fragment to the light
	vec3 toLight = u_LightPos - v_FragPos;
	vec3 lightDir = normalize(toLight);
	lightDir.y = -lightDir.y;

	float distToLight = length(toLight);

	// find the dot product of the light direction and normal
	// which will tell us how lit this fragment is
	float lighting = dot(lightDir, normal);
	lighting = 0.5 * (lighting + 1.0);

	// factor in how intense and far away the light source is
	lighting *= u_LightIntensity * exp(u_LightRange * -distToLight);

	// assign the fragment colour as the sample from the texture multiplied by the lighting value
	gl_FragColor = vec4(lighting * u_LightColor * pixel.rgb, pixel.a);
}
