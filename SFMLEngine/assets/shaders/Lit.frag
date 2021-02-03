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
uniform int u_NumLights;

varying vec3 v_FragPos;

vec3 CalculateDiffuse(int lightIndex, vec3 normal, vec3 worldPos)
{
	// get the direction from the fragment to the light
	vec3 toLight = u_Lights[lightIndex].Position - worldPos;
	vec3 lightDir = normalize(toLight);
	lightDir.y = -lightDir.y;

	float distToLight = length(toLight);

	// find the dot product of the light direction and normal
	// which will tell us how lit this fragment is
	float lighting = dot(lightDir, normal);
	lighting = 0.5 * (lighting + 1.0);

	// factor in how intense and far away the light source is
	lighting *= u_Lights[lightIndex].Intensity * exp(u_Lights[lightIndex].Range * -distToLight);
	
	return lighting * u_Lights[lightIndex].Color.rgb;
}

void main()
{

	// lookup the pixel in the main texture
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	
	// get the normal of the fragment
	vec3 normal = texture2D(u_NormalMap, gl_TexCoord[0].xy).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	vec3 lightColor = vec3(0, 0, 0);
	for (int i = 0; i < u_NumLights; i++)
	{
		lightColor += CalculateDiffuse(i, normal, v_FragPos);
	}

	// assign the fragment colour as the sample from the texture multiplied by the lighting value
	gl_FragColor = vec4(lightColor * pixel.rgb, pixel.a);
}
