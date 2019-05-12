#include <sb7.h>

using namespace std;

// sample 3: indexed rendering
class firstprogram : public sb7::application
{
	void init()
	{
		static const char title[] = "OpenGL geïndexeerd renderen;

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
			"#version 420 core                                             \n"
			"                                                              \n"
			"out vec4 color;                                               \n"
			"                                                              \n"
			"void main(void)                                               \n"
			"{                                                             \n"
			"    color = vec4(1.0f, 0.5f, 0.0f, 1.0f);                     \n"
			"}                                                             \n"
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

		// alle benodigde vertices voor een rechthoek
		float vertices[] = {
			0.5f,  0.5f, 0.0f,  // rechtsboven
			0.5f, -0.5f, 0.0f,  // rechtsonder
			-0.5f, -0.5f, 0.0f, // linksonder
			-0.5f,  0.5f, 0.0f  // linksboven
		};

		// de vertex indices
		unsigned int indices[] = {  // begin vanaf 0
			0, 1, 3,   // eerste driehoek
			1, 2, 3    // tweede driehoek
		};

		glGenVertexArrays(1, &vao); // vertex array object aanmaken	
		glGenBuffers(1, &vbo); // Vertex Buffer Object aanmaken	
		glGenBuffers(1, &ebo); // Element Buffer Object aanmaken	

		// koppel het vertex array object aan de context
		glBindVertexArray(vao);

		// vertex buffer object koppelen aan target (GL_ARRAY_BUFFER)
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// plaats de daadwerkelijke vertex data in het buffer geheugen
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		// ...
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		// ...
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


		// vertex attributen instellen
		glVertexAttribPointer(
			0, // locatie
			3, // aantal waarden
			GL_FLOAT, // data type
			GL_FALSE, 
			3 * sizeof(float), // stride; oftewel de ruimte tussen de vertex attributen
			(void*)0 // begin positie van de buffer data
		);

		glEnableVertexAttribArray(0);

		// unbind
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
		
		// koppel het vertex array object (vierkant) aan de context
		glBindVertexArray(vao);
		
		// geef aan welke primitives gerendert worden uit onze data (speciefiek voor indexed rendering)
		glDrawElements(
			GL_TRIANGLES, // primitive type
			6, // aantal indices
			GL_UNSIGNED_INT, // n.v.t.
			0 
		);
		
		glBindVertexArray(0);
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
		glDeleteProgram(rendering_program);
	}

private:
	GLuint rendering_program; // shader programma ID
	GLuint vao; // Vertex Array Object ID
	GLuint vbo; // Vertex Buffer Object ID
	GLuint ebo; // Element Buffer Object ID
};

// dit bestand is geregistreerd als het opstart punt en voert de opstart logica van sb7 uit
DECLARE_MAIN(firstprogram)

