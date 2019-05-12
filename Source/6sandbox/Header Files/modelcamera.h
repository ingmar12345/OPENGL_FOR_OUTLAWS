#pragma once

#include "camera.h"

// camera die een object volgt
class ModelCamera : public Camera
{
public:
	ModelCamera(Projection proj, Model & model, float offset_distance, float offset_angle);
	~ModelCamera();

	void setOffset(float distance, float angle);

private:
	void initViewMatrix();

	Model & model;
	float offset_distance;   // volg afstand
	float offset_horizontal; // horizontale afstand
	float offset_vertical;   // verticale afstand
};