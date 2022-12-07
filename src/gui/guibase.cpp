#include "guibase.h"

GUIBASE::GUIBASE(GLFWwindow* window) {

	imgui_initialize(window);
}

GUIBASE::~GUIBASE() {

	destroy();
}

void GUIBASE::imgui_initialize(GLFWwindow* window) {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	_guiIO = &ImGui::GetIO();
	reinterpret_cast<void*>(_guiIO);

	_guiIO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	_guiIO->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

}

void GUIBASE::update() {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
}

void GUIBASE::render() const {

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	if (_guiIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

		GLFWwindow* backupCurrentCtx = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backupCurrentCtx);

	}
	
}

void GUIBASE::destroy() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

}

bool GUIBASE::is_capturing_mouse() const {

	return _guiIO->WantCaptureMouse;
}

void GUIBASE::draw_docking_space(void (*draw_menu_bar)()) {

	static ImGuiWindowFlags windowFlags = 
		ImGuiWindowFlags_NoDocking				|
		ImGuiWindowFlags_NoTitleBar				|
		ImGuiWindowFlags_MenuBar				|
		ImGuiWindowFlags_NoCollapse				|
		ImGuiWindowFlags_NoResize				|
		ImGuiWindowFlags_NoMove					|
		ImGuiWindowFlags_NoBringToFrontOnFocus	|
		ImGuiWindowFlags_NoNavFocus				|
		ImGuiWindowFlags_NoBackground;

	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("dockingWindow", nullptr, windowFlags);
	ImGui::PopStyleVar(3);

	ImGui::DockSpace(ImGui::GetID("dockingWindow"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

	if (draw_menu_bar != nullptr)
		draw_menu_bar();

	ImGui::End();

}