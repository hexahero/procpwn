#ifndef SCAN_MEM_POPUP
#define SCAN_MEM_POPUP

void draw_scan_memory_popup(bool* scanMemoryPopup) {

	if (imgui::BeginPopupModal(
		"scanMemoryPopup",
		scanMemoryPopup,
		ImGuiWindowFlags_AlwaysAutoResize	|
		ImGuiWindowFlags_NoTitleBar			|
		ImGuiWindowFlags_NoMove				|
		ImGuiWindowFlags_NoSavedSettings
	)) {

		imgui::Text("Scan Process Memory");
		imgui::Separator();

		static int valToSearchFor;
		imgui::InputInt("Integer", &valToSearchFor, 0);

		if (imgui::Button("Scan", ImVec2(120, 0))) {

			std::string scanProcName = g_dataController.get_hkproc_name(g_processToScan);

			g_logger.add_log(scanProcName + ": scanning for \"" + std::to_string(valToSearchFor) + "\"...", LOGINFO);

			std::thread(
				scan_process_memory<int>,
				g_dataController.get_hkproc_handle(g_processToScan),
				std::ref(g_dataController.get_hkprocs().at(g_processToScan)._scannedInts),
				valToSearchFor,
				[scanProcName]() {
					size_t results = g_dataController.get_hkprocs().at(g_processToScan)._scannedInts.size();
					g_logger.add_log(scanProcName + ": scanning finished (" + std::to_string(results) + " results)", LOGINFO);
				}
			).detach();

			*scanMemoryPopup = false;
			imgui::CloseCurrentPopup();

		}

		imgui::SameLine();

		if (imgui::Button("Cancel", ImVec2(120, 0))) {

			*scanMemoryPopup = false;
			imgui::CloseCurrentPopup();

		}

		imgui::EndPopup();

	}

	imgui::OpenPopup("scanMemoryPopup");

}

#endif