#include "model.h"

using namespace std;

Model::Model(std::vector<float>& vertex_arr, vmath::vec3 model_color)
	: vertex_array(vertex_arr)
{
	this->model_color = model_color;
	
	initVertices();     // initialiseer vertex objecten en pointers
	instance_counter++; // instantie bij op tellen

	// standaard waarden initialiseren
	model_shine = 0.5;                        // glans
	speed = 0.0f;                             // snelheid
	position = vmath::vec3(0.0f, 0.0f, 0.0f); // positie (oorsprong)
	forward_local = forward_default;          // lokale richting voorwaarts
	up_local = up_default;                    // lokale richting omhoog

	// Euler hoeken initialiseren
	angles.roll = 0.0f;
	angles.pitch = 0.0f;
	angles.yaw = 0.0f;

	// oriëntatie quaternion aanmaken op basis van Euler hoeken
	orientation.EulerToQuat(angles); 
	
	// transformatie matrices initialiseren
	translation_matrix = vmath::mat4::identity();
	rotation_matrix = vmath::mat4::identity();
	scaling_matrix = vmath::mat4::identity();
}

Model::Model(std::vector<float>& vertex_arr, vmath::vec3 model_color, float scale)
	: Model(vertex_arr, model_color)
{
	scaling_matrix = vmath::scale(scale, scale, scale);
}

Model::Model(std::vector<float>& vertex_arr, vmath::vec3 model_color, float scale, vmath::vec3 position)
	: Model(vertex_arr, model_color, scale)
{
	this->position = position;
}
Model::~Model()
{
	cleanup();
}

int Model::getNrOfInstances()
{
	return instance_counter;
}

int Model::instance_counter = 0;

void Model::initVertices()
{
	model_size = vertex_array.size();

	glGenVertexArrays(1, &vao); // vertex array object aanmaken	
	glGenBuffers(1, &vbo); // Vertex Buffer Object aanmaken	
 
	// koppel het vertex array object aan de context
	glBindVertexArray(vao); 

	// vertex buffer object koppelen aan de de juiste context (GL_ARRAY_BUFFER)
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// plaats de daadwerkelijke vertex data in het buffer geheugen
	glBufferData(GL_ARRAY_BUFFER, model_size * sizeof(float), &vertex_array[0], GL_STATIC_DRAW);

	// vertex attribuut pointer instellen voor positie
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	// vertex attribuut pointer instellen voor normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// vertex attributen inschakelen
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model::render(GLuint& shader_program, double delta_time)
{
	position += vmath::vec3(forward_local[0], forward_local[1], forward_local[2]) * (delta_time * speed);
	translation_matrix = vmath::translate(position);

	model_matrix = translation_matrix * rotation_matrix * scaling_matrix;

	// matrices en vectors koppelen aan uniforms
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, FALSE, model_matrix);
	glUniform3fv(glGetUniformLocation(shader_program, "modelColor"), 1, model_color);
	glUniform1f(glGetUniformLocation(shader_program, "modelShine"), model_shine);
	
	glBindVertexArray(vao); // koppel het vertex array object aan de context
	glDrawArrays(GL_TRIANGLES, 0, model_size / 2); // teken commando
	glBindVertexArray(0); // ontkoppel het vertex array object
}

void Model::setScale(float factor)
{
	scaling_matrix = vmath::scale(factor, factor, factor);
}
void Model::setPosition(vmath::vec3 position)
{
	this->position = position;
}
void Model::setShine(float factor)
{
	model_shine = factor;
}
vmath::vec3 Model::getForward()
{
	return vmath::vec3(forward_local[0], forward_local[1], forward_local[2]);
}
vmath::vec3 Model::getUp()
{
	return vmath::vec3(up_local[0], up_local[1], up_local[2]);
}
vmath::vec3 Model::getPosition()
{
	return position;
}
vmath::mat4 Model::getModelMatrix()
{
	return model_matrix;
}
float Model::getSpeed()
{
	return speed;
}

void Model::accelerate(float factor)
{
	speed += factor;
}

void Model::rotate(Euler angles)
{
	// "tijdelijk" rotatie quaternion
	Quaternion rotation; 
						 
	// input als Euler hoeken (intuïtief voor gebruiker)
	rotation.EulerToQuat(angles); 

	// oriëntatie quaternion vermenigvuldigen met rotatie quaternion
	orientation = rotation * orientation; 
	
	// rotatie matrix afleiden van oriëntatie quaternion
	rotation_matrix = orientation.getRotationMatrix(); 

	// voorwaartse vector en omhoog vector berekenen
	forward_local = forward_default * rotation_matrix.transpose();
	up_local = up_default * rotation_matrix.transpose();
	normalize(forward_local);
	normalize(up_local);
}

void Model::cleanup()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void Model::printPosition()
{
	string spokes = "\n\nCenter: " + to_string(position[0]) + " " + to_string(position[1]) + " " + to_string(position[2]);
	OutputDebugString(spokes.c_str());
}

