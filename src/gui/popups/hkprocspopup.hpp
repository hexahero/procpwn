#ifndef HK_PROC_POPUP
#define HK_PROC_POPUP

void draw_too_many_hooked_procs_popup(bool* tooManyHProcsPopup) {

	if (imgui::BeginPopupModal(
		"tooManyHProcsPopup",
		tooManyHProcsPopup,
		ImGuiWindowFlags_AlwaysAutoResize	|
		ImGuiWindowFlags_NoTitleBar			|
		ImGuiWindowFlags_NoMove				|
		ImGuiWindowFlags_NoSavedSettings
	)) {

		imgui::Text("Error");
		imgui::Separator();
		imgui::Text("\n\tMax number of hooked processes: %d\t\n\n", MAX_PROCESSES);

		if (imgui::Button("OK", ImVec2(120, 0))) {

			*tooManyHProcsPopup = false;
			imgui::CloseCurrentPopup();

		}

		imgui::EndPopup();

	}

	imgui::OpenPopup("tooManyHProcsPopup");

}

#endif