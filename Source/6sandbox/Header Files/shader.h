#pragma once

#include <sb7.h>
#include <fstream>
#include <string>

// shader klasse
class Shader
{
public:
	Shader(const char * vertex, const char * fragment);
	Shader(const char * vertex, const char * fragment, const char * geometry);
	~Shader();  

	GLuint getProgram();

private:
	// koppel functie voor shader programma
	GLuint createShader(const char * path, GLuint target);
	std::string readFile(const char * path); // bestand inlezen
	void compileShader(GLuint shader); // compile specifieke shader
	void attachShaders(); // koppel shaders aan het shader programma
	void deleteShaders(); // verwijder shaders

	void setCapabilities(); // openGL instellingen
	 
	GLuint program;
	GLuint vertex_shader, fragment_shader, geometry_shader;
};

