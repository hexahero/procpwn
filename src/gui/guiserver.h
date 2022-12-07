#ifndef MAINGUI
#define MAINGUI

#include "guibase.h"
#include "../window/window.h"

#define imgui ImGui

static void draw_menu_bar();

class GUISERVER : public GUIBASE {

public:

	GUISERVER() = delete;
	GUISERVER(const GUISERVER&) = delete;
	GUISERVER& operator = (const GUISERVER&) = delete;

	// Construct
	GUISERVER(WINDOW& window);

	// Draw GUI
	void draw();

private:

	// General
	WINDOW& _window;

	// on/off states
	bool _processesList;
	bool _hookedProcessesList;
	bool _logView;
	bool _memoryViewer;
	bool _watchView;

	// Styling
	ImGuiStyle& _style;
	ImVec4*		_color;

};

#endif