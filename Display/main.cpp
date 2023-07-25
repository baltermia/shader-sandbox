// gl
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std
#include <iostream>
#include <vector>

// lib
#include "Helper/Helper.h"

void callback_framebuffer_size_changed(GLFWwindow* window, int width, int height);

void frame_process_input();
void frame_render();

void init_memory();
void init_shaders();

GLFWwindow* window;

GLuint program,
	   vao,
	   ebo,
	   vbo,
	   window_size_uniform,
	   width,
	   height;

int main()
{
	width = 800;
	height = 600;

	/* --------------------------------- */
	/* Initialize GLFW */

	// initialize glfw
	glfwInit();

	// set OpenGL version
	// check installed version with a tool such as https://realtech-vr.com/home/glview
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	// explicitly use core version (so no major backwards-compatibility)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// set property specific on macos systems (note that opengl 4.6 doesnt work on macos, so make sure to downgrade it above)
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* --------------------------------- */
	/* Create Window Object */

	window = glfwCreateWindow(width, height, "Shader Sandbox", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW Window object" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	/* --------------------------------- */
	/* Initialize GLAD */
	// this is strictly neccessary before using any OpenGL function

	// glfwGetProcAddress defis the correct function pointers based on which OS the code is compiling for
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/* --------------------------------- */
	/* Configure OpenGL */

	// offset (0,0)
	glViewport(0, 0, width, height);

	// create callback which changes the gl-viewport automatically when the glfw-window gets resized
	glfwSetFramebufferSizeCallback(window, callback_framebuffer_size_changed);

		
	/* --------------------------------- */
	/* Create opengl program */

	program = glCreateProgram();

	init_memory();
	
	init_shaders();

	/* --------------------------------- */
	/* Render-Loop */

	while (!glfwWindowShouldClose(window))
	{
		frame_process_input();

		frame_render();

		// double buffer
		glfwSwapBuffers(window);

		// checks for events
		glfwPollEvents();
	}

	/* --------------------------------- */
	/* Clean Up / Deallocate */
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);

	glDeleteVertexArrays(1, &vao);

	glDeleteProgram(program);
	glfwTerminate();

	/* --------------------------------- */

}

/* Callbacks */

void callback_framebuffer_size_changed(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/* ------------------------------------- */
/* Functions that get called for each frame */

void frame_process_input()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void frame_render()
{
	// use vao
	glUseProgram(program);
	glBindVertexArray(vao);

	// draw
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void init_memory()
{
	// vao
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	/* ------------------------------------- */
	// Write Memory to GPU
	std::vector<vec3f> vertices =
	{
		{ -1, 1 },	// top left
		{ 1, 1 },	// top right
		{ -1, -1 },	// bottom left
		{ 1, -1 }	// bottom right
	};

	std::vector<vec3ui> indices =
	{
		{ 0, 1, 2 },	// first triangle
		{ 1, 2, 3 }		// second triangle
	};

	// ebo
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices.front()), indices.data(), GL_STATIC_DRAW);

	// vbo
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.front()), vertices.data(), GL_STATIC_DRAW);

	/* ------------------------------------- */
	// Vertex-Linking Attributes

	glVertexAttribPointer(0, sizeof(vec3f) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(vec3f), (void*)NULL);
	glEnableVertexAttribArray(0);

	// unbind
	glBindVertexArray(0);
}

void init_shaders()
{
	bool success;

	// Load Fragment Shader
	GLuint fragment_shader;
	success = help::try_load_shader("shader.frag", &fragment_shader, GL_FRAGMENT_SHADER);

	// Create fragment shader unfirom
	window_size_uniform = glGetUniformLocation(fragment_shader, "window_size");

	// Load Shader into Program

	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	success = help::check_linking_success(program);

	glDeleteShader(fragment_shader);
}

/* ------------------------------------- */
