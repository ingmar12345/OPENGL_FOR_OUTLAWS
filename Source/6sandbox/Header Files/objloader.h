#pragma once

#include <Windows.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <vmath.h>

// vertex data
struct vertexdata
{
	
	std::vector< vmath::vec3 > vertices; // tijdelijke array voor vertex posities
	std::vector< vmath::vec3 > normals;  // tijdelijke array voor vertex normals
	std::vector< vmath::vec2 > uvs;      // tijdelijke array voor texture coördinaten
};

// index data
struct indexdata
{
	std::vector< vmath::vec3 > vertexIndices; // tijdelijke array voor vertex positie indices
	std::vector< vmath::vec3 > normalIndices; // tijdelijke array voor vertex normals
	std::vector< vmath::vec3 > uvIndices;     // tijdelijke array voor texture coördinaten
};

class Objloader
{
public:
	// functie om een OBJ bestand rechtstreeks om te zetten in een vertex array
	static std::vector<float> * getVerticesFromFile(const char * filepath);

private:
	Objloader();
	~Objloader();

	static std::vector<float> * data;

	// lees het bestand in en maak tijdelijke objecten voor de vertex data en de index data
	static bool load(const char * path);

	// structureer de vertex data arrays in een nieuwe, voor OpenGL bruikbare, array
	static std::vector<float> * vertexArrayGenerator(vertexdata& verts, indexdata& indics);
};