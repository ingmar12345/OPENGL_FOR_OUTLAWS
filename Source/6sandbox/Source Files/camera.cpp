#include "camera.h"

Camera::Camera(Projection projection_params)
{
	this->proj = projection_params;
	initProjMatrix();
}
Camera::~Camera() {}

// creëer een view matrix
void Camera::initViewMatrix()
{
	view_matrix = vmath::lookat(
		camera_pos,  // camera positie
		look_pos,    // kijkpunt
		up           // omhoog vector
	);
}
// creëer een projectie matrix
void Camera::initProjMatrix()
{
	proj_matrix = vmath::perspective(
		proj.view_angle_degrees,            // kijkhoek graden
		proj.aspect_ratio,                  // beeld verhouding
		proj.near_plane,                    // minimale render afstand
		proj.far_plane                      // maximale render afstand
	);
}

vmath::vec3 Camera::getCameraPosition()
{
	return camera_pos;
}
vmath::mat4 Camera::getViewMatrix()
{
	return view_matrix;
}
vmath::mat4 Camera::getProjectionMatrix()
{
	return proj_matrix;
}

// kijkhoek aanpassen
void Camera::setFov(float fovy)
{
	proj.view_angle_degrees = fovy;
	initProjMatrix();
}

void Camera::attach(int shader_program)
{
	initViewMatrix(); // view matrix updaten
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, FALSE, view_matrix);
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, FALSE, proj_matrix);
}
