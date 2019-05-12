#version 420 core

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNorm;

out vec4 vertColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 modelColor;
uniform float modelShine;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(void)
{
	// vertex positie in clip space berekenen
	gl_Position = projection * view * model * vec4(vertPos, 1.0f);

	// vertex positie in world space berekenen
	vec3 vertPosWorld = vec3(model * vec4(vertPos, 1.0f));

	// normal in world space berekenen
	// schaling en translatie elimineren uit normal m.b.v. getransponeerde inverse model matrix
	vec3 vertNormWorld = mat3(transpose(inverse(model))) * vertNorm;

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

	vertColor = vec4(result, 1.0f);
}