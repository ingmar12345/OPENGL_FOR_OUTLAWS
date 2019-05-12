#include "light.h"

Light::Light(vmath::vec3 light_pos, vmath::vec3 view_pos)
{
	this->light_pos = light_pos;
	this->view_pos = view_pos;
}
Light::~Light() {}

void Light::setLightPos(const vmath::vec3 pos)
{
	light_pos = pos;
}

void Light::setViewPos(const vmath::vec3 pos)
{
	view_pos = pos;
}

// koppel licht parameters als uniforms aan een shader programma
void Light::attach(int shader_program)
{
	glUniform3fv(glGetUniformLocation(shader_program, "lightPos"), 1, light_pos);
	glUniform3fv(glGetUniformLocation(shader_program, "viewPos"), 1, view_pos);
}