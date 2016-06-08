/*
OpenGL ES 2.0
Built-In Variable	Precision	Data Type

Vertex Shader Built-In Variables
gl_Position			highp		vec4
gl_FrontFacing		-			bool
gl_PointSize		mediump		float

Fragment Shader Built-In Variables
gl_FragColor		mediump		vec4
gl_FrontFacing		-			bool
gl_FragCoord		mediump		vec4
gl_PointCoord		mediump		vec2
*/

#define DEBUG


// std shit
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <map>

#include <cmath>

// Rendering shit
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
// glm::vec3
#include <glm/vec3.hpp>
// glm::vec4, glm::ivec4
#include <glm/vec4.hpp>
// glm::mat4
#include <glm/mat4x4.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

//
#include <FileReader.h>
#include <FragmentShader.h>
#include <ShaderProgram.h>
#include <VertexShader.h>
#include <TextureLoader.h>
#include <CameraProcessor.h>
#include <Transform.h>
#include <Util.h>

//
#include <Mesh.h>
#include <MeshLoader.h>
#include <MeshRenderer.h>
#include <PathUtil.h>

#define TITLE "Hans die Wurst"
#define PI_OVER_2 1.57079632679f

struct GameData {

	GLFWwindow *window;
	bool is_running;
	ShaderProgram program;

	struct {
		GLint vertex_position;
		GLint vertex_uv;
		GLint vertex_normal;
		GLint vertex_color;
	} attributes;

	int window_width, window_height;
	int framebuffer_width, framebuffer_height;

	TextureLoader texture_loader;
	blurryroots::model::MeshLoader mesh_loader;
	blurryroots::model::MeshRenderer mesh_renderer;

	struct {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
	} matrices;

	float light_radius;
	float light_intensity;
	glm::vec3 light_color;
	float light_color_factor;
	float light_color_base;
	bool complex_attenuation;

	Transform models[4];

	CameraProcessor camera_processor;

	bool fullscreen;

} game_data;

void
test_obj_loader (void);

//
void
initialize (void);

void
on_update (double dt);

void
on_render ();

void
dispose ();

// Callbacks
void
on_key (GLFWwindow *window, int key, int scancode, int action, int mods);

void
on_framebuffer (GLFWwindow *window, int width, int height);

void
on_cursor_position (GLFWwindow *window, double xpos, double ypos);

void
on_cursor_enter (GLFWwindow *window, int entered);

void
on_mouse_button (GLFWwindow *window, int button, int action, int mods);

void
on_scroll (GLFWwindow *window, double xoffset, double yoffset);

int
main (void) {
	try {
		// stuff to setup
		initialize ();
		DEBUG_LOG ("Finished initialization.");

		// Loop until the user closes the window
		double lastTime = glfwGetTime ();
		while (game_data.is_running) {
			// Calculate time spend processing the last frame
			double deltaTime = glfwGetTime () - lastTime;
			lastTime = glfwGetTime ();

			// Do logical updates
			on_update (deltaTime);

			// Draw stuff onto screen
			on_render ();

			// Swap front and back buffers
			glfwSwapBuffers (game_data.window);

			// Poll for and process events
			glfwPollEvents ();
		}
	}
	catch (std::exception &ex) {
		std::cout << "Cought: " << ex.what () << std::endl;
	}
	catch (...) {
		std::cout << "Cought unkown exception :(" << std::endl;
	}

	dispose ();
	DEBUG_WARN ("Finished disposing.");

	return 0;
}

void
open_window (GameData &ctx, const std::string &title, bool fullscreen) {
	// Initialize GLFW
	THROW_IF (GL_TRUE != glfwInit (),
		"Could not initialize glfw!"
	);

	std::cout
		<< "Build with GLFW version "
		<< GLFW_VERSION_MAJOR << "."
		<< GLFW_VERSION_MINOR << "."
		<< GLFW_VERSION_REVISION << std::endl;
	{
		int major, minor, revision;
		glfwGetVersion (&major, &minor, &revision);
		THROW_IF (GLFW_VERSION_MAJOR != major,
			"GLFW major version differs! Runtime: ",
			std::to_string (GLFW_VERSION_MAJOR), " "
			"Build: ", std::to_string (major)
		);
		THROW_IF (GLFW_VERSION_MINOR != minor,
			"GLFW minor version differs! Runtime: ",
			std::to_string (GLFW_VERSION_MINOR), " "
			"Build: ", std::to_string (minor)
		);
		THROW_IF (GLFW_VERSION_REVISION != revision,
			"GLFW revision version differs! Runtime: ",
			std::to_string (GLFW_VERSION_REVISION), " "
			"Build: ", std::to_string (revision)
		);
	}

	GLFWmonitor *monitor = glfwGetPrimaryMonitor ();
	THROW_IF (nullptr == monitor,
		"Could not primary monitor!"
	);

	const GLFWvidmode *mode = glfwGetVideoMode (monitor);
	THROW_IF (nullptr == mode,
		"Could not get video mode for primary monitor!"
	);
	glfwWindowHint (GLFW_RED_BITS, mode->redBits);
	glfwWindowHint (GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint (GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint (GLFW_REFRESH_RATE, mode->refreshRate);
	glfwWindowHint (GLFW_VISIBLE, GL_FALSE);

	glfwWindowHint (GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);

	ctx.window_width = mode->width / 2;
	ctx.window_height = mode->height / 2;
	// Create a window and its OpenGL context
	ctx.window = glfwCreateWindow (
		ctx.window_width, ctx.window_height,
		title.c_str (),
		fullscreen ? monitor : nullptr,
		nullptr // OpendGL context sharing
	);
	THROW_IF (nullptr == ctx.window,
		"Could not create window!"
	);
	ctx.fullscreen = fullscreen;

	// THIS HAS TO BE CALLED BEFORE GLEW GETS LOADED!
	// Make the window's context current
	glfwMakeContextCurrent (ctx.window);

	// Extension wrangler initialising
	glewExperimental = GL_TRUE;
	GLuint glew_status = glewInit ();
	THROW_IF (GLEW_OK != glew_status,
		"Could not initialize glew!"
	);

	std::cout
		<< "OpenGL Version: " << glGetString (GL_VERSION) << "\n"
		<< "OpenGL Shader : " << glGetString (GL_SHADING_LANGUAGE_VERSION)
		<< std::endl
		;

	glfwShowWindow (ctx.window); {
		glfwGetFramebufferSize (ctx.window,
			&(ctx.framebuffer_width), &(ctx.framebuffer_height)
		);
		std::cout
			<< "Inital framebuffer size: "
			<< ctx.framebuffer_width << " / " << ctx.framebuffer_height
			;
		glViewport (0, 0, ctx.framebuffer_width, ctx.framebuffer_height);
	}

	// callbacks
	glfwSetKeyCallback (ctx.window, on_key);
	glfwSetFramebufferSizeCallback (ctx.window, on_framebuffer);
	glfwSetCursorPosCallback (ctx.window, on_cursor_position);
	glfwSetCursorEnterCallback (ctx.window, on_cursor_enter);
	glfwSetMouseButtonCallback (ctx.window, on_mouse_button);
	glfwSetScrollCallback (ctx.window, on_scroll);

	glEnable (GL_ALPHA_TEST);
	glAlphaFunc (GL_GREATER, 0.0f);

	// Enable alpha
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LESS);

	glEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);
	glFrontFace (GL_CCW);
}

void
initialize (void) {
	//
	open_window (game_data, TITLE, false);

	// setup game stuff
	game_data.is_running = true;

	//std::string basePath = "C:/Users/klabusterbeere/Workspace/Remote/hannelore/bin/Debug/";
	std::string basePath = get_executable_path ();

	const std::string from = "\\";
	const std::string to = "/";
	size_t start_pos = 0;
	while ((start_pos = basePath.find (from, start_pos)) != std::string::npos) {
		basePath.replace (start_pos, from.length (), to);
		// In case 'to' contains 'from', like replacing 'x' with 'yx'
		start_pos += to.length ();
	}
	basePath += "/";

	// setup basic shaders
	FileReader vertFile (basePath + "shaders/es/basic.vert");
	std::string vsText = vertFile.to_string ();
	THROW_IF (0 == vsText.size (),
		"Vertex shader is missing or empty!"
	);
	auto vs = VertexShader (vertFile.to_string ());

	FileReader fragFile (basePath + "shaders/es/basic.frag");
	std::string fragText = fragFile.to_string ();
	THROW_IF(0 == fragText.size (),
		"Fragment shader is missing or empty!"
	);
	auto fs = FragmentShader (fragFile.to_string ());

	game_data.program = ShaderProgramBuilder ()
		.add_shader (vs)
		.add_shader (fs)
		.bind_attribute ("vertex_position", 0)
		.bind_attribute ("vertex_uv", 1)
		.bind_attribute ("vertex_normal", 2)
		.link ()
		;

	game_data.texture_loader.load (basePath + "textures/ground.lines.png", "ground", 0);
	game_data.mesh_loader.load (
		basePath + "models/objs/ground.obj", game_data.program, "ground"
	);

	game_data.texture_loader.load (basePath + "textures/grass.png", "suzanne", 0);
	game_data.mesh_loader.load (
		basePath + "models/objs/suzanne.smooth.obj", game_data.program, "suzanne"
	);
	game_data.models[1].translate (glm::vec3 (0, 0.5, 1));
	game_data.models[1].rotate (-PI_OVER_2 * 2.0f, Transform::UP);

	game_data.texture_loader.load (basePath + "textures/light.uv.png", "light", 0);
	game_data.mesh_loader.load (
		basePath + "models/objs/light_sphere.obj", game_data.program, "light_sphere"
	);
	game_data.models[2].translate (glm::vec3 (0, 2, -2));

	//
	game_data.texture_loader.load (basePath + "textures/sky.jpg", "sky", 0);
	game_data.mesh_loader.load (
		basePath + "models/objs/skysphere.obj", game_data.program, "sky_sphere"
	);
	game_data.models[3].translate (glm::vec3 (0, 0, 0));
	game_data.models[3].scale (glm::vec3 (4, 4, 4));
	{
		auto *ground = game_data.mesh_loader.get ("ground");
		float max_ground_dim = ground->dimensions[0].x;
		max_ground_dim = glm::max (ground->dimensions[0].y,
			max_ground_dim
		);
		max_ground_dim = glm::max (ground->dimensions[0].z,
			max_ground_dim
		);
		game_data.models[3].scale (glm::vec3 (max_ground_dim * glm::sqrt (2)));
	}

	game_data.camera_processor.on_initialize ();
	game_data.camera_processor.transform.translate (glm::vec3 (0, 6, -6));
	game_data.camera_processor.transform.rotate (
		-PI_OVER_2 * 1.5f, Transform::UP
	);
	glm::mat4 inv_rotation = glm::inverse (
		game_data.camera_processor.transform.to_rotation ()
	);
	glm::vec3 right = Transform::to_right (inv_rotation);
	game_data.camera_processor.transform.rotate (PI_OVER_2 * 0.5f, right);
	game_data.camera_processor.on_viewport_changed(
		game_data.framebuffer_width, game_data.framebuffer_height
	);

	game_data.light_radius = 1.0f;
	game_data.light_intensity = 2.0f;
	game_data.light_color = glm::vec3 (1.0, 0.8, 0.8);
	game_data.complex_attenuation = true;
}

float suzanne_speed = 0.8f;
void
on_update (double dt) {
	game_data.is_running = game_data.is_running
		&& ! glfwWindowShouldClose (game_data.window)
		;

	game_data.camera_processor.on_update (dt);

	glm::vec3 pos = Transform::to_position (
		game_data.models[1].to_translation ()
	);
	if (-6 > pos.z || 2 < pos.z) {
		suzanne_speed *= -1;
	}

	float speed = suzanne_speed * dt;
	game_data.models[1].translate (glm::vec3 (0, 0, speed));

	// light size according to radius
	game_data.models[2].reset_scale ();
	game_data.models[2].scale (glm::vec3 (game_data.light_radius));
}

void
render_model (
	const blurryroots::model::Mesh *mesh,
	const Transform &transform,
	const std::string &texture_key,
	TextureLoader &texture_loader,
	ShaderProgram &program
) {
	texture_loader.bind (texture_key);
	glBindVertexArray (mesh->vertex_array_object);

	// calculate and forward mesh transform
	program.set_uniform_mat4 ("m",
		transform.to_matrix ()
	);

	int size;
	glGetBufferParameteriv (
		GL_ELEMENT_ARRAY_BUFFER,
		GL_BUFFER_SIZE,
		&size
	);
	THROW_IF (0 >= size,
		"Invalid element buffer!"
	);

	// draw all the triangles!
	int element_count = size / sizeof (mesh->shapes[0].mesh.indices.at (0));
	int real_element_count = mesh->shapes[0].mesh.indices.size ();
	THROW_IF (element_count != real_element_count,
		"Unequal element_count ", std::to_string (element_count),
		" vs ", std::to_string (real_element_count)
	);
	glDrawElements (GL_TRIANGLES, element_count, GL_UNSIGNED_INT, 0);

	glBindVertexArray (0);
	texture_loader.unbind (texture_key);
}

void
on_render () {
	glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	game_data.program.use ();

	game_data.camera_processor.on_render (game_data.program);

	// ambient light
	game_data.program.set_uniform_i ("complex_attenuation",
		static_cast<int> (game_data.complex_attenuation)
	);

	// ambient light
	game_data.program.set_uniform_vec3 ("ambient_light",
		glm::vec3 (0.08, 0.08, 0.2)
	);

	// point light
	game_data.program.set_uniform_vec4 ("point_light_color",
		glm::vec4 (game_data.light_color, game_data.light_intensity)
	);
	game_data.program.set_uniform_vec4 ("point_light",
		glm::vec4 (
			Transform::to_position (game_data.models[2].to_translation ()),
			game_data.light_radius
		)
	);

	render_model (
		game_data.mesh_loader.get ("ground"),
		game_data.models[0],
		"ground",
		game_data.texture_loader,
		game_data.program
	);

	render_model (
		game_data.mesh_loader.get ("suzanne"),
		game_data.models[1],
		"suzanne",
		game_data.texture_loader,
		game_data.program
	);

	render_model (
		game_data.mesh_loader.get ("light_sphere"),
		game_data.models[2],
		"light",
		game_data.texture_loader,
		game_data.program
	);

	render_model (
		game_data.mesh_loader.get ("sky_sphere"),
		game_data.models[3],
		"sky",
		game_data.texture_loader,
		game_data.program
	);

	game_data.program.deactivate ();
}

void
dispose () {
	game_data.program.dispose ();

	game_data.texture_loader.dispose ();

	game_data.mesh_loader.dispose ();

	glfwTerminate ();
}

bool change_intensity = false;
// Callbacks
void
on_key (GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (GLFW_KEY_ESCAPE == key && GLFW_RELEASE == action) {
		game_data.is_running = false;
	}

	game_data.camera_processor.on_key (key, scancode, action, mods);

	if (GLFW_KEY_SPACE == key) {
		if (GLFW_RELEASE == action) {
			std::cout
				<< "Light radius @ "
				<< game_data.light_radius
				<< std::endl;
		}
	}

	if (mods & GLFW_MOD_CONTROL) {
		std::cout << "intensity toggle" << std::endl;
		change_intensity = ! change_intensity;
	}

	if (mods & GLFW_MOD_ALT) {
		std::cout << "attenuation toggle" << std::endl;
		game_data.complex_attenuation = ! game_data.complex_attenuation;
	}
}

void
on_framebuffer (GLFWwindow *window, int width, int height) {
	std::cout << "framebuffer changed from"
		<< "\nw: " << game_data.window_width << " to " << width
		<< "\nh: " << game_data.window_height << " to " << height
		<< "\non window " << window
		<< std::endl;
		;
	game_data.framebuffer_width = width;
	game_data.framebuffer_height = height;

	glViewport (0, 0,
		game_data.framebuffer_width, game_data.framebuffer_height
	);
	game_data.camera_processor.on_viewport_changed (
		game_data.framebuffer_width, game_data.framebuffer_height
	);
}

void
on_cursor_position (GLFWwindow *window, double xpos, double ypos) {
	//
	game_data.camera_processor.on_cursor_position (xpos, ypos);
}

void
on_cursor_enter (GLFWwindow *window, int entered) {
	if (entered) {
		// The cursor entered the client area of the window
		game_data.camera_processor.activate ();
	}
	else {
		// The cursor left the client area of the window
		game_data.camera_processor.deactivate ();
	}
}

void
on_mouse_button (GLFWwindow *window, int button, int action, int mods) {
	//
}

void
on_scroll (GLFWwindow *window, double xoffset, double yoffset) {
	//
	if (change_intensity) {
		game_data.light_intensity += (float)yoffset / 10.f;
		game_data.light_intensity = game_data.light_intensity < 0
			? 0
			: game_data.light_intensity
			;
		std::cout << "intensity @ " << game_data.light_intensity << std::endl;
	}
	else {
		game_data.light_radius += (float)yoffset / 10.f;
		game_data.light_radius = game_data.light_radius < 0
			? 0
			: game_data.light_radius
			;
		std::cout << "light_radius @ " << game_data.light_radius << std::endl;
	}
}
