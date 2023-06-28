// gl
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std
#include <iostream>

void callback_framebuffer_size_changed(GLFWwindow* window, int width, int height);

void frame_process_input(GLFWwindow* window);
void frame_render();

int main()
{
	size_t width = 800;
	size_t height = 600;

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

	GLFWwindow* window = glfwCreateWindow(width, height, "Shader Sandbox", NULL, NULL);

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
	/* Render-Loop */

	while (!glfwWindowShouldClose(window))
	{
		frame_process_input(window);

		frame_render();

		// double buffer
		glfwSwapBuffers(window);

		// checks for events
		glfwPollEvents();
	}

	/* --------------------------------- */
	/* Clean Up / Deallocate */

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

void frame_process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void frame_render()
{
	// color that gets replaced with
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// clear all colors
	glClear(GL_COLOR_BUFFER_BIT);
}

/* ------------------------------------- */
