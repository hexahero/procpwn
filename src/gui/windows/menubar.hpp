#ifndef MENUBAR
#define MENUBAR

#include <thread>

void draw_menu_bar() {

	if (imgui::BeginMenuBar()) {

		if (imgui::BeginMenu("File")) {

			if (imgui::MenuItem("Demo window", NULL, false)) {

				if (g_isDemoWindowEnabled) g_isDemoWindowEnabled = false;
				else g_isDemoWindowEnabled = true;

			}

			if (imgui::MenuItem("Dump logs", NULL, false)) {

				g_logger.dump_logs();
			}

			imgui::Separator();

			imgui::EndMenu();
		}

		if (imgui::BeginMenu("Processes")) {

			if (imgui::MenuItem("Load processes", NULL, false))
				obtain_processes_from_system(g_dataController.get_procs());

			imgui::EndMenu();
		}

		if (imgui::BeginMenu("Memory")) {
			

			imgui::EndMenu();
		}

		if (imgui::BeginMenu("Window")) {


			imgui::EndMenu();
		}

		if (imgui::BeginMenu("Settings")) {

			if (imgui::MenuItem("Console", NULL, false)) {

				if (g_isConsoleEnabled) {
					ShowWindow(g_console, SW_HIDE);
					g_isConsoleEnabled = false;
				}
				else {
					ShowWindow(g_console, SW_SHOW);
					g_isConsoleEnabled = true;
				}	
				
			}

			imgui::EndMenu();
		}

		imgui::EndMenuBar();
	}

}

#endif