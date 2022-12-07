#ifndef WND
#define WND

#include <iostream>

#define GLFW_INCLUDE_NONE

#include <glad/include/glad/glad.h>
#include <glfw/include/glfw3.h>

class WINDOW {

public:

	WINDOW() = delete;
	WINDOW(const WINDOW&) = delete;
	WINDOW& operator = (const WINDOW&) = delete;

	WINDOW(const char* wintitle, unsigned int winX, unsigned int winY);

	// Main loop
	void start_render_loop(void (*update)());
	void shutdown();

	GLFWwindow* get_handle();

private:

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void (*_updateptr)();

	bool _terminate;

	void initialize();
	void configure();
	void bind();
	void cleanup();

	// Window data
	GLFWwindow* _window;

	const char*		_wintitle;
	unsigned int	_winX;
	unsigned int	_winY;

};

#endif