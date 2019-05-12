#pragma once

#include <sb7.h>
#include <vmath.h>

// licht
class Light
{
public:
	Light(vmath::vec3 light_pos, vmath::vec3 view_pos);
	~Light();

	void setLightPos(vmath::vec3 pos);
	void setViewPos(vmath::vec3 pos);

	void attach(int shader_program);

private:
	vmath::vec3 light_pos;
	vmath::vec3 view_pos;
};