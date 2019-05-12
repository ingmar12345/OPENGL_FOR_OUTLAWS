#pragma once

#include "camera.h"

// camera die een object volgt vanuit een statische positie
class RcCamera : public Camera
{
public:
	RcCamera(Projection proj, Model & model, vmath::vec3 camera_pos, vmath::vec3 up);
	~RcCamera();

private:
	void initViewMatrix();

	Model & model;
	float zoomfactor;
	float min_fov;      // minimale frustum hoek
	float min_distance; // minimale kijk afstand
};