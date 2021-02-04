uniform sampler2D texture;

uniform sampler2D u_NormalMap;

// --------------
// POINT LIGHTING
// --------------

struct PointLightData
{
	vec3 Position;
	float Intensity;
	float Range;
	vec4 Color;
};
uniform PointLightData u_PointLights[8];
uniform int u_NumPointLights;

// --------------------
// DIRECTIONAL LIGHTING
// --------------------

struct DirectionalLightData
{
	vec3 Direction;
	float Intensity;
	vec4 Color;
};
uniform DirectionalLightData u_DirectionalLights[2];
uniform int u_NumDirectionalLights;


uniform float u_Rotation;


varying vec3 v_FragPos;

vec3 CalculatePointLight(int lightIndex, vec3 normal, vec3 worldPos)
{
	// get the direction from the fragment to the light
	vec3 toLight = u_PointLights[lightIndex].Position - worldPos;
	vec3 lightDir = normalize(toLight);
	lightDir.y = -lightDir.y;

	float distToLight = length(toLight);

	// find the dot product of the light direction and normal
	// which will tell us how lit this fragment is
	float lighting = dot(lightDir, normal);
	lighting = 0.5 * (lighting + 1.0);

	// factor in how intense and far away the light source is
	lighting *= u_PointLights[lightIndex].Intensity * exp(u_PointLights[lightIndex].Range * -distToLight);
	
	return lighting * u_PointLights[lightIndex].Color.rgb;
}

vec3 CalculateDirectionalLight(int lightIndex, vec3 normal)
{
	// find the dot product of the light direction and the normal
	float lighting = dot(normalize(u_DirectionalLights[lightIndex].Direction), normal);
	lighting = 0.5 * (lighting + 1.0);

	// factor in the intensity of the light source
	lighting *= u_DirectionalLights[lightIndex].Intensity;

	return lighting * u_DirectionalLights[lightIndex].Color.rgb;
}

void main()
{

	// lookup the pixel in the main texture
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	
	// get the normal of the fragment
	vec3 normal = texture2D(u_NormalMap, gl_TexCoord[0].xy).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	// if the sprite is rotated then we want to transform the normal by the rotation
	normal = vec3(normal.x * cos(u_Rotation) - normal.y * sin(u_Rotation), normal.x * sin(u_Rotation) + normal.y * cos(u_Rotation), normal.z);

	// the accumulative affect of each light on this pixel
	vec3 lightColor = vec3(0, 0, 0);

	// calculate the diffuse light from directional lights
	for (int i = 0; i < u_NumDirectionalLights; i++)
	{
		lightColor += CalculateDirectionalLight(i, normal);
	}

	// calculate the diffuse light from point lights
	for (int i = 0; i < u_NumPointLights; i++)
	{
		lightColor += CalculatePointLight(i, normal, v_FragPos);
	}

	// assign the fragment colour as the sample from the texture multiplied by the lighting value
	gl_FragColor = vec4(lightColor * pixel.rgb, pixel.a);
}
