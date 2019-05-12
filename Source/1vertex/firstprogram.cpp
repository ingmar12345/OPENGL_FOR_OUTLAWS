#include <sb7.h>

// sample 1: vertex tekenen
class firstprogram : public sb7::application
{
	void init()
	{
		static const char title[] = "OpenGL vertex";

		sb7::application::init();

		memcpy(info.title, title, sizeof(title));
	}

	// shader programma maken en compilen
	GLuint compile_shaders(void)
	{
		static const char * vs_source[] =
		{
			"#version 420 core                             \n"
			"                                              \n"
			"void main(void)                               \n"
			"{                                             \n"
			"    gl_Position = vec4(0.0, 0.0, 0.5, 1.0);   \n"
			"}                                             \n"
		};

		static const char * fs_source[] =
		{
			"#version 420 core                             \n"
			"                                              \n"
			"out vec4 color;                               \n"
			"                                              \n"
			"void main(void)                               \n"
			"{                                             \n"
			"    color = vec4(0.0, 0.8, 1.0, 1.0);         \n"
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

		// vertex array object aanmaken
		glGenVertexArrays(
			1,    // grootte
			&vao  // object ID
		);

		// koppel de ID van het vertex array object aan de context
		glBindVertexArray(vao);
	}

	

	// deze functie zorgt voor het renderen
	virtual void render(double currentTime)
	{
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f); // achtergrond kleur
		glClear(GL_COLOR_BUFFER_BIT); // framebuffer opschonen	


		
		// koppel het shader programma aan de context
		glUseProgram(rendering_program); 
		
		glPointSize(40.0f);

		// geef aan welke primitives gerendert worden uit onze data
		glDrawArrays(
			GL_POINTS, // primitive type
			0, // n.v.t.
			1 // aantal vertices die getekent worden
		);
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteProgram(rendering_program);
	}

private:
	GLuint rendering_program; // shader programma ID
	GLuint vao; // Vertex Array Object ID
};

// de enige instantie van DECLARE MAIN 
// dit bestand is geregistreerd als het opstart punt en voert de opstart logica van sb7 uit
DECLARE_MAIN(firstprogram)