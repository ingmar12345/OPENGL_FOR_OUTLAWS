#include <sb7.h>

using namespace std;

// sample 2: driehoek tekenen
class firstprogram : public sb7::application
{
	void init()
	{
		static const char title[] = "OpenGL driehoek";

		sb7::application::init();

		memcpy(info.title, title, sizeof(title));
	}

	// shader programma maken en compilen
	GLuint compile_shaders(void)
	{
		static const char * vs_source[] =
		{
			"#version 420 core                                             \n"
			"                                                              \n"
			"layout (location = 0) in vec3 vertPos;                        \n"
			"                                                              \n"
			"void main(void)                                               \n"
			"{                                                             \n"
			"    gl_Position = vec4(vertPos.x, vertPos.y, vertPos.z, 1.0f);\n"
			"}                                                             \n"
		};

		static const char * fs_source[] =
		{
			"#version 420 core                             \n"
			"                                              \n"
			"out vec4 color;                               \n"
			"                                              \n"
			"void main(void)                               \n"
			"{                                             \n"
			"    color = vec4(1.0f, 0.5f, 0.0f, 1.0f);     \n"
			"}                                             \n"
		};

		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vs_source, NULL);
		glCompileShader(vertex_shader);

		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fs_source, NULL);
		glCompileShader(fragment_shader);

		// compile vertex shader
		int  success_vs;
		char infoLog_vs[512];
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success_vs);
		if (!success_vs)
		{
			glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog_vs);
			OutputDebugString(infoLog_vs);
		}

		// compile fragment shader
		int  success_fs;
		char infoLog_fs[512];
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success_fs);
		if (!success_fs)
		{
			glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog_fs);
			OutputDebugString(infoLog_fs);
		}

		GLuint program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);
		OutputDebugString("shaders linked");

		// check of de shaders correct gelinkt zijn
		int  success_link;
		char infoLog_link[512];
		glGetProgramiv(program, GL_LINK_STATUS, &success_link);
		if (!success_link) {
			glGetProgramInfoLog(program, 512, NULL, infoLog_link);
			OutputDebugString(infoLog_link);
		}

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	virtual void startup()
	{
		// laat compile_shaders een shader programma genereren
		rendering_program = compile_shaders();

		// gehardcode driehoek
		float vertices[] = {
			-0.5f, -0.6f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
		};

		// vertex array object aanmaken
		glGenVertexArrays(
			1,    // aantal vertex array objecten
			&vao  // object ID
		);

		// Vertex Buffer Object aanmaken
		glGenBuffers(
			1,   // aantal buffer objecten
			&vbo // object ID
		);

		// koppel het vertex array object aan de context
		glBindVertexArray(vao);

		// koppel de ID van het buffer object aan de de juiste target (GL_ARRAY_BUFFER)
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// plaats de daadwerkelijke array met data in het GPU geheugen
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// vertex attribuut instellen voor positie
		glVertexAttribPointer(
			0,                 // index; later nodig in shader
			3,                 // aantal waardes; X, Y en Z
			GL_FLOAT,          // data type
			GL_FALSE,          // normaliseren ja/nee
			3 * sizeof(float), // geheugen grootte (3 keer 32 bits float)
			(void*)0           // offset; begin vanaf ...
		);

		// activeer vertex attribuut met index 0
		glEnableVertexAttribArray(0);

		// ontkoppelen
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


	}



	// deze functie zorgt voor het renderen
	virtual void render(double currentTime)
	{
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f); // achtergrond kleur
		glClear(GL_COLOR_BUFFER_BIT); // framebuffer opschonen	


		// koppel het shader programma aan de context
		glUseProgram(rendering_program);

		// koppel het vertex array object van de driehoek aan de context
		glBindVertexArray(vao);

		// geef aan welke primitives gerendert worden uit onze data
		glDrawArrays(
			GL_TRIANGLES, // primitive type
			0, // n.v.t.
			3 // aantal vertices die getekent worden
		);
		
		// ontkoppel het vertex array object
		glBindVertexArray(0);
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteProgram(rendering_program);
	}

private:
	GLuint rendering_program; // shader programma ID
	GLuint vao; // Vertex Array Object ID
	GLuint vbo; // Vertex Buffer Object ID
};

// dit bestand is geregistreerd als het opstart punt en voert de opstart logica van sb7 uit
DECLARE_MAIN(firstprogram)