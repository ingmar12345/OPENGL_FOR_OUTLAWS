#pragma once

#include "camera.h"

// statische camera
class StaticCamera : public Camera
{
public:
	StaticCamera(Projection proj, vmath::vec3 camera_pos, vmath::vec3 look_pos, vmath::vec3 up);
	~StaticCamera();

	void setCameraPosition(vmath::vec3 new_pos);
};