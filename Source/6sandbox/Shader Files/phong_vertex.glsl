#version 420 core

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNorm;

out vec3 vertPosWorld;
out vec3 vertNormWorld;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
	// vertex positie in clip space berekenen
	gl_Position = projection * view * model * vec4(vertPos, 1.0f);

	// vertex positie in world space berekenen
	vertPosWorld = vec3(model * vec4(vertPos, 1.0f));

	// normal in world space berekenen
	// schaling en translatie elimineren uit normal m.b.v. getransponeerde inverse model matrix
	vertNormWorld = mat3(transpose(inverse(model))) * vertNorm;
}