#include "window/window.h"
#include "gui/guiserver.h"

const char* wintitle = "procpwn";
constexpr unsigned int WINX = 1500;
constexpr unsigned int WINY = 900;

WINDOW		window(wintitle, WINX, WINY);
GUISERVER	gui(window);

void update() {

	gui.draw();
}

int main() {

	window.start_render_loop(update);
	
	return 0;
}