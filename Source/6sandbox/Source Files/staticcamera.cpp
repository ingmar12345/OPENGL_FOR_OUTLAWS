#include "staticcamera.h"

StaticCamera::StaticCamera(Projection proj, vmath::vec3 camera_pos, vmath::vec3 look_pos, vmath::vec3 up) 
	: Camera(proj)
{
	this->camera_pos = camera_pos;
	this->look_pos = look_pos;
	this->up = up;

	initViewMatrix();
}
StaticCamera::~StaticCamera() {}

void StaticCamera::setCameraPosition(vmath::vec3 new_pos)
{
	camera_pos = vmath::vec3(new_pos);
	initViewMatrix();
}