#version 410 core

in vec3 FragPos;
in vec3 v_vertexColor;
in vec3 v_vertexNormal;
in vec2 v_textureCoordinates;
in mat3 TBN;

const int MAX_LIGHTS = 10;

struct Light {
	vec3 positions[MAX_LIGHTS];
	vec3 colors[MAX_LIGHTS];
	float constant;
	float linear;
	float quadratic;
};

uniform Light u_lights;
uniform int u_lightCount;
uniform vec3 u_viewPosition;

// from professor Shah's example code
uniform sampler2D u_DiffuseTexture;
// normal map coordinates
uniform sampler2D u_BumpMap;

out vec4 color;

// Entry point of program
void main()
{
	vec3 diffuseColor = texture(u_DiffuseTexture, v_textureCoordinates).rgb;

	// Used modified sample code from https://learnopengl.com/Lighting/Basic-Lighting
	vec3 ambient = vec3(0.0f);
	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);
	float attenuation = 1.0;

    vec3 norm = texture(u_BumpMap, v_textureCoordinates).rgb; // get normal from normal map
	norm = norm * 2.0 - 1.0; // scale to [-1, 1]
	norm *= 1.5; // Scale the normal values to make them more visible
	norm = clamp(norm, -1.0, 1.0); // Keep values in range
	norm = normalize(TBN * norm); // transform to tangent space

	norm = normalize(norm);

	for (int i = 0; i < u_lightCount; i++) {
		vec3 lightPos = u_lights.positions[i];
		vec3 lightColor = u_lights.colors[i];

		// Ambient lighting
		float ambientStrength = 0.2;
		ambient += ambientStrength * lightColor;

		// Diffuse lighting
		vec3 lightDir = normalize(lightPos - FragPos);  
		float diff = max(dot(norm, lightDir), 0.0);
		diffuse += diff * lightColor;

		// Specular lighting
		float specularStrength = 0.3;
		vec3 viewDir = normalize(u_viewPosition - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		specular += specularStrength * spec * lightColor;  

		// Calculate attenuation
		float distance = length(lightPos - FragPos);
		float attenuationStrength = .5;
		attenuation = 1.0 / (u_lights.constant + u_lights.linear * distance +
		u_lights.quadratic * (distance * distance));
		attenuation *= 1 / attenuationStrength;

	}
	// Apply lighting
	vec3 result = (ambient + diffuse + specular) * diffuseColor;

	// Apply attenuation
	result *= attenuation;

	color = vec4(result, 1.0f);
}