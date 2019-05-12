#version 420 core

in vec3 vertPosWorld;
in vec3 vertNormWorld;
out vec4 color;

uniform vec3 modelColor;
uniform float modelShine;
uniform vec3 lightPos;
uniform vec3 viewPos;


void main(void)
{
	// ambient component
	float ambientFactor = 0.3f;
	vec3 ambient = ambientFactor * modelColor;

	// diffuse component
	vec3 lightVec = normalize(lightPos - vertPosWorld);
	vec3 normVec = normalize(vertNormWorld);
	float diffuseFactor = max(dot(normVec, lightVec), 0.0f);
	vec3 diffuse = diffuseFactor * modelColor;

	// specular component
	vec3 viewVec = normalize(viewPos - vertPosWorld);
	vec3 reflectVec = reflect(-lightVec, normVec);
	float specFactor = pow(max(dot(viewVec, reflectVec), 0.0f), 16);
	float specShine = modelShine;
	vec3 specColor = vec3(1.0f, 1.0f, 1.0f); 
	vec3 specular = specShine * specFactor * specColor;

	// resultaat
	vec3 result = ambient + diffuse + specular;

	color = vec4(result, 1.0f);
}