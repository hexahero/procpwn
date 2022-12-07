#include "window.h"

void (*WINDOW::_updateptr)() = nullptr;

WINDOW::WINDOW(const char* wintitle, unsigned int winX, unsigned int winY)
	:
	_terminate(false),
	_wintitle(wintitle),
	_winX(winX),
	_winY(winY)
{

	initialize();
}

void WINDOW::start_render_loop(void (*update)()) {

	_updateptr = update;

	while (!glfwWindowShouldClose(_window) && !_terminate) {

		glClear(GL_COLOR_BUFFER_BIT);

		update();

		glfwSwapBuffers(_window);
		glfwPollEvents();

	}

	cleanup();

}

void WINDOW::shutdown() {

	_terminate = true;
}

GLFWwindow* WINDOW::get_handle() {

	return _window;
}

void WINDOW::initialize() {

	// Initialize GLFW
	glfwInit();

	// Configure future window
	configure();

	// Initialize window
	_window = glfwCreateWindow(_winX, _winY, _wintitle, NULL, NULL);
	glfwSetWindowSizeCallback(_window, framebuffer_size_callback);

	// Error check
	if (_window == NULL) {

		std::cout << "Initialization error\n";
		glfwTerminate();

		return;
	}

	// Bind window to current context and load GLAD
	bind();

	glfwSetWindowPos(_window, 100, 100);
	glfwSetWindowSizeLimits(_window, 700, 500, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_MULTISAMPLE);

}

void WINDOW::configure() {

	// Specify opengl version and enable core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	glfwSwapInterval(1);

}

void WINDOW::bind() {

	glfwMakeContextCurrent(_window);
	gladLoadGL();

}

void WINDOW::cleanup() {

	glfwDestroyWindow(_window);
	glfwTerminate();

}

void WINDOW::framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);

	_updateptr();

	glfwSwapBuffers(window);
	glfwPollEvents();

}