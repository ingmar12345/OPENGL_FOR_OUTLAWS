#pragma once

#include <sb7.h>
#include <vmath.h>
#include <vector>
#include <Windows.h>
#include <string>
#include "quaternion.h"

#define PI 3.14159265

// alle logica voor een te renderen 3D model
class Model
{
public:
	Model(std::vector<float>& vertex_array, vmath::vec3 model_color);
	Model(std::vector<float>& vertex_array, vmath::vec3 model_color, float scale);
	Model(std::vector<float>& vertex_array, vmath::vec3 model_color, float scale, vmath::vec3 position);
	~Model();

	// render logica
	void render(GLuint& shader_program, double delta_time);

	// opruim protocol
	void cleanup();
	
	// properties
	void setScale(float factor);
	void setPosition(vmath::vec3 position);
	void setShine(float factor);
	vmath::vec3 getForward();
	vmath::vec3 getUp();
	vmath::vec3 getPosition();
	vmath::mat4 getModelMatrix();
	static int getNrOfInstances();
	float getSpeed();

	// handig voor debuggen
	void printPosition();

	// interactie functies
	void accelerate(float delta);
	void rotate(Euler delta);

private:
	// initialiseert vertex objecten en pointers
	void initVertices();

	static int instance_counter; // modellen teller

	// model specifieke VAO, VBO en shader programma
	GLuint vao, vbo, shader_program; 

	std::vector<float>& vertex_array; // vector met vertex attributen
	int model_size;                   // aantal vertices
	vmath::vec3 model_color;          // model kleur
	float model_shine;                // glans
	
		
	// transformaties matrices
	vmath::mat4 translation_matrix, rotation_matrix, scaling_matrix; 

	// model matrix
	vmath::mat4 model_matrix; 

	// standaard richtingen (forward (Z-as), up (Y-as))
	const vmath::vec4 forward_default = vmath::vec4(0.0f, 0.0f, 1.0f, 1.0f); 
	const vmath::vec4 up_default = vmath::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	vmath::vec4 forward_local, up_local; 

	Quaternion orientation; // orientatie quaternion
	Euler angles;           // Euler hoeken (alleen voor initialisatie)
	vmath::vec3 position;   // positie
	float speed;            // snelheid
};