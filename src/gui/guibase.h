#ifndef BASEGUI
#define BASEGUI

#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glad/include/glad/glad.h>
#include <glfw/include/glfw3.h>

class GUIBASE {

public:

	GUIBASE() = delete;
	GUIBASE(const GUIBASE&) = delete;
	GUIBASE& operator = (const GUIBASE&) = delete;

	// Coustruct/setup
	GUIBASE(GLFWwindow* window);
	~GUIBASE();
	void imgui_initialize(GLFWwindow* window);
	
	void update();
	void render() const;
	void destroy();

	bool is_capturing_mouse() const;

protected:

	void draw_docking_space(void (*draw_menu_bar)() = nullptr);

	ImGuiIO* _guiIO;

};

#endif