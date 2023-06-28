// gl
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/* --------------------------------- */
	/* Render-Loop */

	while (!glfwWindowShouldClose(window))
	{
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}