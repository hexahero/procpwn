#include "../memnproc/proc.h"
#include "../memnproc/procmem.h"

#include "guiserver.h"
#include "../util/util.h"
#include "../util/logger.hpp"
#include "../memnproc/datacontroller.hpp"

HWND g_console				= GetConsoleWindow();
bool g_isConsoleEnabled		= false;
bool g_isDemoWindowEnabled	= false;

unsigned int g_processToScan = 0;

bool g_tooManyHkProcsPopup	= false;
bool g_scanMemoryPopup		= false;

LOGGER			g_logger;
DATACONTROLLER	g_dataController;

#include "windows/menubar.hpp"
#include "windows/proclist.hpp"
#include "windows/hkproclist.hpp"
#include "windows/logview.hpp"
#include "windows/memviewer.hpp"
#include "windows/watchview.hpp"

#include "popups/hkprocspopup.hpp"
#include "popups/scanmempopup.hpp"

GUISERVER::GUISERVER(WINDOW& window)
	:
	GUIBASE(window.get_handle()),
	_window(window),
	_processesList(true),
	_hookedProcessesList(true),
	_logView(true),
	_memoryViewer(true),
	_watchView(true),
	_style(ImGui::GetStyle()),
	_color(_style.Colors)
{

	const char* fontname = "resources/Roboto-Regular.ttf";
	const char* fontpath = concat_root_dir_path(fontname, (int)strlen(fontname));

	_guiIO->Fonts->AddFontFromFileTTF(fontpath, 16);

	if (g_isConsoleEnabled)
		ShowWindow(g_console, SW_SHOW);
	else
		ShowWindow(g_console, SW_HIDE);

	_color[ImGuiCol_TitleBg]		= { 0.14f, 0.14f, 0.14f, 1.0f };
	_color[ImGuiCol_TitleBgActive]	= { 0.14f, 0.14f, 0.14f, 1.0f };
	
}

void GUISERVER::draw() {

	update();

	draw_docking_space(draw_menu_bar);

	if (_processesList)			draw_processes_list(&_processesList);
	if (_hookedProcessesList)	draw_hooked_processes_list(&_hookedProcessesList);
	if (_logView)				draw_log_window(&_logView);
	if (_memoryViewer)			draw_memory_viewer(&_memoryViewer);
	if (_watchView)				draw_watch_view(&_watchView);
	
	if (g_scanMemoryPopup)		draw_scan_memory_popup(&g_scanMemoryPopup);
	if (g_tooManyHkProcsPopup)	draw_too_many_hooked_procs_popup(&g_tooManyHkProcsPopup);

	if (g_isDemoWindowEnabled)	imgui::ShowDemoWindow();

	render();

}