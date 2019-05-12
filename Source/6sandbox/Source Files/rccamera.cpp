#include "rccamera.h"

RcCamera::RcCamera(Projection proj, Model & model, vmath::vec3 camera_pos, vmath::vec3 up) 
	: Camera(proj), model(model)
{
	this->camera_pos = camera_pos;
	this->up = up;
	min_fov = proj.view_angle_degrees;
	min_distance = vmath::distance(camera_pos, model.getPosition());

	initViewMatrix();
}
RcCamera::~RcCamera() {}

void RcCamera::initViewMatrix()
{
	float current_distance = vmath::distance(camera_pos, model.getPosition());
	if (current_distance < min_distance)
	{
		current_distance = min_distance;
	}
	zoomfactor = min_distance / current_distance;
	setFov(min_fov * zoomfactor);
	look_pos = model.getPosition();

	Camera::initViewMatrix();
}