#version 420 core

layout (location = 0) in vec3 vertPos;

out vec3 vertPosWorld;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
	// vertex positie in clip space berekenen
	gl_Position = projection * view * model * vec4(vertPos, 1.0f);

	// vertex positie in world space berekenen
	vertPosWorld = vec3(model * vec4(vertPos, 1.0f));
}