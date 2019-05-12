#pragma once

#include <vmath.h>
#include "model.h"

// struct met benodigde parameters van projectie matrix
struct Projection
{
	float view_angle_degrees; // frustum hoek in graden
	float aspect_ratio;       // beeldverhouding
	float near_plane;         // minimale render afstand
	float far_plane;          // maximale render afstand
};

// basis klasse camera
class Camera
{
public:
	// koppel functie voor shader programma
	void attach(int shader_program);
	
	// properties
	vmath::mat4 getViewMatrix();
	vmath::mat4 getProjectionMatrix();
	vmath::vec3 getCameraPosition();
	void setFov(float angle_degrees);        // kijkhoek aanpassen       

protected:
	Camera(Projection proj);
	~Camera();

	virtual void initViewMatrix(); // update functie view matrix
	void initProjMatrix();         // update functie projectie matrix
	
	vmath::vec3 camera_pos, look_pos, up; // ingrediënten view matrix
	Projection proj;                      // ingrediënten projectie matrix
	vmath::mat4 view_matrix, proj_matrix; // transformatie matrices
};

