#include "objloader.h"

using namespace std;

Objloader::Objloader()
{
}
Objloader::~Objloader()
{
	delete data;
}

// haal vertex data op uit een gegeven OBJ bestand
std::vector<float> * Objloader::getVerticesFromFile(const char * filepath)
{
	if (Objloader::load(filepath) == true)
	{
		return Objloader::data;
	}
	else
	{
		OutputDebugString("\n Ongeldig bestand");
		return new std::vector<float>(12);
	}
}

std::vector<float> * Objloader::data = nullptr;

bool Objloader::load(const char * path)
{
	vertexdata * vertices = new vertexdata;
	indexdata * indices = new indexdata;

	FILE * file = 0;
	errno_t error = fopen_s(&file, path, "r");
		
	if (file == NULL || error != 0) {
		printf("\n Bestand niet mogelijk om te openen");
		return false;
	}

	while (1) {

		char line_first[128];
		// lees het eerste woord op een regel
		int res = fscanf_s(file, "%s", line_first, sizeof(line_first));
		if (res == EOF)
			break; // EOF = End Of File
		// vertex positie
		if (strcmp(line_first, "v") == 0)
		{
			vmath::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
			vertices->vertices.push_back(vertex);
		}
		// texture coördinaat
		else if (strcmp(line_first, "vt") == 0)
		{
			vmath::vec2 uv;
			fscanf_s(file, "%f %f\n", &uv[0], &uv[1]);
			vertices->uvs.push_back(uv);
		}
		// vertex normal
		else if (strcmp(line_first, "vn") == 0)
		{
			vmath::vec3 normal;
			fscanf_s(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
			vertices->normals.push_back(normal);
		}
		// driehoek / polygoon
		else if (strcmp(line_first, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("\n Bestand kan niet gelezen worden");
				return false;
			}

			indices->vertexIndices.push_back(vmath::vec3(vertexIndex[0]-1, vertexIndex[1]-1, vertexIndex[2]-1));
			indices->normalIndices.push_back(vmath::vec3(normalIndex[0]-1, normalIndex[1]-1, normalIndex[2]-1));
			indices->uvIndices.push_back(vmath::vec3(uvIndex[0]-1, uvIndex[1]-1, uvIndex[2]-1));
		}
	}

	data = vertexArrayGenerator(*vertices, *indices);
	return true;
}

std::vector<float> * Objloader::vertexArrayGenerator(vertexdata& verts, indexdata& indics)
{
	std::vector<float> * vertex_array = new std::vector<float>;

	// model heeft meedere driehoeken (faces)
	for (unsigned int face_index = 0; face_index <  indics.vertexIndices.size(); face_index++)
	{			
		// vertex heeft 3 indices: positie, normaal vector en texture coordinaat
		for (unsigned int vertex_index = 0; vertex_index < 3; vertex_index++) 
		{
			// vertex data
			// face index; vertex index (zoveelste vertex in face); vector index (xyz)
			vertex_array->push_back(verts.vertices[indics.vertexIndices[face_index][vertex_index]][0]); // x coordinaat
			vertex_array->push_back(verts.vertices[indics.vertexIndices[face_index][vertex_index]][1]); // y coordinaat
			vertex_array->push_back(verts.vertices[indics.vertexIndices[face_index][vertex_index]][2]); // z coordinaat

			vertex_array->push_back(verts.normals[indics.normalIndices[face_index][vertex_index]][0]); // x normaal
			vertex_array->push_back(verts.normals[indics.normalIndices[face_index][vertex_index]][1]); // y normaal
			vertex_array->push_back(verts.normals[indics.normalIndices[face_index][vertex_index]][2]); // z normaal

			// momenteel gebruiken we geen texture coordinaten
			//vertex_array->push_back(verts.uvs[indics.uvIndices[face_index][vertex_index]][0]); // x texture coordinaat
			//vertex_array->push_back(verts.uvs[indics.uvIndices[face_index][vertex_index]][1]); // y texture coordinaat
		}
	}

	delete(&indics);
	delete(&verts);
		
	return vertex_array;
}