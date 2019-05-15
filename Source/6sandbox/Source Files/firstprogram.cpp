#include <sb7.h>
#include <vmath.h>
#include "objloader.h"
#include "shader.h"
#include "model.h"
#include "rccamera.h"
#include "modelcamera.h"
#include "staticcamera.h"
#include "light.h"

using namespace std;

// sample 6: sandbox
class firstprogram : public sb7::application
{
	virtual void init()
	{
		static const char title[] = "OpenGL sandbox";

		sb7::application::init();

		memcpy(info.title, title, sizeof(title));
	}

	// initialiseer 3D modellen
	void initModels()
	{
		// vertex arra
		std::vector<float> * plane_vertices = Objloader::getVerticesFromFile("rcplane2.obj");
		std::vector<float> * cartesian_vertices = Objloader::getVerticesFromFile("axis_opengl.obj");
		std::vector<float> * hill_vertices = Objloader::getVerticesFromFile("hillsv3.obj");
		std::vector<float> * building_vertices = Objloader::getVerticesFromFile("buildingsv3.obj");

		// renderbare objecten
		boeing_737_MAX8 = new Model(*plane_vertices, vmath::vec3(1.0f, 0.0f, 0.0f), fixed_scale);
		cartesian_axis = new Model(*cartesian_vertices, vmath::vec3(0.0f, 0.0f, 1.0f), fixed_scale);
		hills = new Model(*hill_vertices, vmath::vec3(0.2f, 0.6f, 0.0f), fixed_scale, scenery_position);
		buildings = new Model(*building_vertices, vmath::vec3(0.7f, 0.7f, 1.0f), fixed_scale, scenery_position);
		boeing_737_MAX8->setShine(1.5f); // vliegtuig heeft glanzender oppervlak
		cartesian_axis->setShine(1.5f);
		hills->setShine(0.0f);

		// opgeslagen in GPU geheugen, niet langer nodig op de heap
		delete(plane_vertices);
		delete(cartesian_vertices);
		delete(hill_vertices);
		delete(building_vertices);
	}

	// initialiseer cameras
	void initCameras()
	{
		Projection projection; // projectie parameters of 'lens'

							   // projectie parameters instellen
		projection.view_angle_degrees = 90.0f;  // frustum hoek
		projection.aspect_ratio = 16 / 9;         // beeldverhouding / aspect ratio                
		projection.near_plane = 0.2f;           // minimale afstand frustum
		projection.far_plane = 10000.0f;      // maximale afstand frustum

											  // stilstaande camera
		static_camera = new StaticCamera(
			projection,                    // projectie parameters of 'lens'
			vmath::vec3(2.0f, 2.0f, 2.0f), // camera positie
			vmath::vec3(0.0f, 0.0f, 0.0f), // kijkpunt
			vmath::vec3(0.0f, 1.0f, 0.0f)  // omhoog vector
		);

		// camera die object volgt
		thirdperson_camera = new ModelCamera(
			projection,       // projectie parameters of 'lens'
			*boeing_737_MAX8, // model
			1.0f,             // volg afstand
			10.0f             // volg hoek
		);

		// camera die object volgt vanuit een vaste positie
		rc_camera = new RcCamera(
			projection,                    // projectie parameters of 'lens'
			*boeing_737_MAX8,              // model
			vmath::vec3(2.0f, 4.0f, 2.0f), // kijkpunt
			vmath::vec3(0.0f, 1.0f, 0.0f)  // omhoog vector
		);

		// actieve camera
		active_camera = static_camera;
	}

	// initialiseer belictingsbron
	void initLighting()
	{
		light_source = new Light(
			vmath::vec3(1.0f, 400.0f, 100.0f), // licht positie
			vmath::vec3(200, 400, 200)         // kijk positie
		);
	}

	// initialiseer shaders
	void initShaders()
	{
		// shaders
		phong = new Shader("phong_vertex.glsl", "phong_fragment.glsl");
		gourad = new Shader("gourad_vertex.glsl", "gourad_fragment.glsl");
		flat = new Shader("flat_vertex.glsl", "flat_fragment.glsl", "flat_geometry.glsl");
		depth = new Shader("depth_vertex.glsl", "depth_fragment.glsl", "depth_geometry.glsl");
		active_shader = phong->getProgram(); // standaard shader
	}

	// gebruikersinput verwerken m.b.t. oriëntatie object
	void handleRotation(double delta_time)
	{
		Euler angles;
		if (pitch_down == true)
		{
			angles.pitch = (float)delta_time * -delta_angle;
		}
		if (pitch_up == true)
		{
			angles.pitch = (float)delta_time * delta_angle;
		}
		if (yaw_left == true)
		{
			angles.yaw = (float)delta_time * -delta_angle;
		}
		if (yaw_right == true)
		{
			angles.yaw = (float)delta_time * delta_angle;
		}
		if (roll_left == true)
		{
			angles.roll = (float)delta_time * delta_angle;
		}
		if (roll_right == true)
		{
			angles.roll = (float)delta_time * -delta_angle;
		}
		boeing_737_MAX8->rotate(angles);
	}

	// eenmalig uitvoeren bij opstarten
	virtual void startup()
	{
		glEnable(GL_DEPTH_TEST); // depth test inschakelen, overlapping detecteren
		glEnable(GL_CULL_FACE); // backfaces negeren

		initShaders();
		initModels();
		initCameras();
		initLighting();

		depth_range = 800.0f; // bereik; depth buffer visualisatie shader
		depth_offset = 0.2f;  // minimale afstand; depth buffer visualisatie shader
		oldTime = 0;
	}

	// logica per render iteratie
	virtual void render(double currentTime)
	{
		glClearColor(0.6f, 0.9f, 1.0f, 1.0f); // achtergrond kleur

											  // framebuffer opschonen (depth buffer en color buffer)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(active_shader); // shader programma toewijzen

									 // maak de depth visualisatie parameters beschikbaar in de shader
		glUniform1f(glGetUniformLocation(active_shader, "depthRange"), depth_range);
		glUniform1f(glGetUniformLocation(active_shader, "depthOffset"), depth_offset);

		delta_time = currentTime - old_time;
		old_time = currentTime;

		handleRotation(delta_time); // rotaties uitvoeren op basis van tijd

		light_source->attach(active_shader); // licht koppelen aan het shader programma				
		active_camera->attach(active_shader); // camera koppelen aan het shader programma

											  // render logica uitvoeren voor model instanties
		boeing_737_MAX8->render(active_shader, delta_time);
		cartesian_axis->render(active_shader, delta_time);
		hills->render(active_shader, delta_time);
		buildings->render(active_shader, delta_time);
	}

	// afsluit protocol
	virtual void shutdown()
	{
		glDeleteProgram(active_shader);
		delete(phong);
		delete(gourad);
		delete(flat);

		delete(boeing_737_MAX8);
		delete(cartesian_axis);
		delete(hills);
		delete(buildings);

		delete(light_source);

		delete(static_camera);
		delete(thirdperson_camera);
		delete(rc_camera);
	}

	// venster schalen
	virtual void onResize(int w, int h)
	{
		glViewport(0, 0, w, h);
	}

	// input bindings
	virtual void onKey(int key, int action)
	{
		switch (key)
		{
		case GLFW_KEY_UP:
			if (action == GLFW_PRESS)
			{
				pitch_down = true;
			}
			else if (action == GLFW_RELEASE)
			{
				pitch_down = false;
			}
			break;
		case GLFW_KEY_DOWN:
			if (action == GLFW_PRESS)
			{
				pitch_up = true;
			}
			else if (action == GLFW_RELEASE)
			{
				pitch_up = false;
			}
			break;
		case GLFW_KEY_LEFT_BRACKET:
			if (action == GLFW_PRESS)
			{
				yaw_left = true;
			}
			else if (action == GLFW_RELEASE)
			{
				yaw_left = false;
			}
			break;
		case GLFW_KEY_RIGHT_BRACKET:
			if (action == GLFW_PRESS)
			{
				yaw_right = true;
			}
			else if (action == GLFW_RELEASE)
			{
				yaw_right = false;
			}
			break;
		case GLFW_KEY_LEFT:
			if (action == GLFW_PRESS)
			{
				roll_left = true;
			}
			else if (action == GLFW_RELEASE)
			{
				roll_left = false;
			}
			break;
		case GLFW_KEY_RIGHT:
			if (action == GLFW_PRESS)
			{
				roll_right = true;
			}
			else if (action == GLFW_RELEASE)
			{
				roll_right = false;
			}
			break;
		case GLFW_KEY_W:
			boeing_737_MAX8->accelerate(delta_speed);
			break;
		case GLFW_KEY_S:
			boeing_737_MAX8->accelerate(-delta_speed);
			break;
		case GLFW_KEY_C:
			boeing_737_MAX8->printPosition();
			break;
		case GLFW_KEY_1:
			active_camera = static_camera;
			break;
		case GLFW_KEY_2:
			active_camera = thirdperson_camera;
			break;
		case GLFW_KEY_3:
			active_camera = rc_camera;
			break;
		case GLFW_KEY_P:
			active_shader = phong->getProgram();
			break;
		case GLFW_KEY_G:
			active_shader = gourad->getProgram();
			break;
		case GLFW_KEY_F:
			active_shader = flat->getProgram();
			break;
		case GLFW_KEY_D:
			active_shader = depth->getProgram();
			break;
		case GLFW_KEY_MINUS:
			if (depth_range < 1.0f)
			{
				depth_range = 1.0f;
			}
			depth_range -= 10.0f;
			break;
		case GLFW_KEY_EQUAL:
			depth_range += 10.0f;
			break;
		case GLFW_KEY_PAGE_DOWN:
			depth_offset += 0.01f;
			break;
		case GLFW_KEY_PAGE_UP:
			depth_offset -= 0.01f;
			break;
		default:
			break;
		}
	}

private:
	// shaders
	Shader * phong;
	Shader * gourad;
	Shader * flat;
	Shader * depth;
	float depth_range;
	float depth_offset;
	GLuint active_shader;

	// belichtings parameters
	Light * light_source;

	// camera en projectie
	StaticCamera * static_camera;
	ModelCamera * thirdperson_camera;
	RcCamera * rc_camera;
	Camera * active_camera;

	// renderbare object(en)
	Model * boeing_737_MAX8;
	Model * cartesian_axis;
	Model * hills;
	Model * buildings;

	const float delta_angle = 80.0f;
	const float delta_speed = 0.5f;
	double old_time, delta_time;

	const vmath::vec3 scenery_position = vmath::vec3(0.0f, -60.0f, 0.0f); // positie van het landschap
	const float fixed_scale = 1.0f / 1000.0f; // schaal 1 : 1000

	bool pitch_up = false;
	bool pitch_down = false;
	bool yaw_left = false;
	bool yaw_right = false;
	bool roll_left = false;
	bool roll_right = false;
};

DECLARE_MAIN(firstprogram)