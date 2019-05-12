#include "shader.h"

Shader::Shader(const char * vertex_path, const char * fragment_path)
{
	program = glCreateProgram();

	vertex_shader = createShader(vertex_path, GL_VERTEX_SHADER);
	fragment_shader = createShader(fragment_path, GL_FRAGMENT_SHADER);
	geometry_shader = 0;

	compileShader(vertex_shader);
	compileShader(fragment_shader);

	attachShaders();
	deleteShaders();
}
Shader::Shader(const char * vertex_path, const char * fragment_path, const char * geometry_path)
{
	program = glCreateProgram();

	vertex_shader = createShader(vertex_path, GL_VERTEX_SHADER);
	fragment_shader = createShader(fragment_path, GL_FRAGMENT_SHADER);
	geometry_shader = createShader(geometry_path, GL_GEOMETRY_SHADER);

	compileShader(vertex_shader);
	compileShader(fragment_shader);
	compileShader(geometry_shader);

	attachShaders();
	deleteShaders();
}

Shader::~Shader(){}

GLuint Shader::createShader(const char * path, GLuint target)
{
	std::string source_s = readFile(path);
	const GLchar * source = source_s.c_str();
	GLuint shader = glCreateShader(target);
	glShaderSource(shader, 1, &source, NULL);
	return shader;
}

std::string Shader::readFile(const char * path)
{
	std::string content;

	std::ifstream fileStream(path, std::ios::in);

	if (!fileStream.is_open())
	{
		OutputDebugString("\n kan bestand niet lezen");
		return "foo";
	}

	std::string line = "";

	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::compileShader(GLuint shader)
{
	glCompileShader(shader);
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		OutputDebugString(infoLog);
	}
}

void Shader::attachShaders()
{
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	if (geometry_shader != 0)
		glAttachShader(program, geometry_shader);

	glLinkProgram(program);

	int  success_link;
	char infoLog_link[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success_link);
	if (!success_link) {
		glGetProgramInfoLog(program, 512, NULL, infoLog_link);
		OutputDebugString(infoLog_link);
	}
}

void Shader::deleteShaders()
{
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	if (geometry_shader != 0)
		glDeleteShader(geometry_shader);
}

GLuint Shader::getProgram()
{
	return program;
}
