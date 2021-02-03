uniform sampler2D texture;

uniform sampler2D u_NormalMap;

struct LightData
{
	vec3 Position;
	float Intensity;
	float Range;
	vec4 Color;
};
uniform LightData u_Lights[16];

varying vec3 v_FragPos;

void main()
{

	// lookup the pixel in the main texture
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	
	// get the normal of the fragment
	vec3 normal = texture2D(u_NormalMap, gl_TexCoord[0].xy).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	// get the direction from the fragment to the light
	vec3 toLight = u_Lights[0].Position - v_FragPos;
	vec3 lightDir = normalize(toLight);
	lightDir.y = -lightDir.y;

	float distToLight = length(toLight);

	// find the dot product of the light direction and normal
	// which will tell us how lit this fragment is
	float lighting = dot(lightDir, normal);
	lighting = 0.5 * (lighting + 1.0);

	// factor in how intense and far away the light source is
	lighting *= u_Lights[0].Intensity * exp(u_Lights[0].Range * -distToLight);

	// assign the fragment colour as the sample from the texture multiplied by the lighting value
	gl_FragColor = vec4(lighting * u_Lights[0].Color * pixel.rgb, pixel.a);
}
