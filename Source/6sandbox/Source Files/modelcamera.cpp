#include "modelcamera.h"

ModelCamera::ModelCamera(Projection proj, Model & model, float offset_distance, float angle_degrees) 
	: Camera(proj), model(model)
{
	this->offset_distance = offset_distance;
	offset_horizontal = -1.0f;
	offset_vertical = tan(-angle_degrees * PI / 180) * offset_horizontal;

	initViewMatrix();
}
ModelCamera::~ModelCamera() {}

void ModelCamera::initViewMatrix()
{
	vmath::vec3 offsetHorizontal = vmath::normalize(model.getForward()) * offset_horizontal;
	vmath::vec3 offsetVertical = vmath::normalize(model.getUp()) * offset_vertical;
	vmath::vec3 offset = vmath::normalize(offsetHorizontal + offsetVertical);
	camera_pos = model.getPosition() + (offset * offset_distance);
	
	up = vmath::normalize(model.getUp());

	look_pos = model.getPosition();
	
	Camera::initViewMatrix();
}

void ModelCamera::setOffset(float distance, float angle_degrees)
{
	offset_vertical = tan(-angle_degrees * PI / 180) * offset_horizontal;
	offset_distance = distance;
	initViewMatrix();
}
