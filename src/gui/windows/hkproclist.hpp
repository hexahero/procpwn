#ifndef HKPROCLIST
#define HKPROCLIST

void draw_hooked_processes_list(bool* hookedProcessesList);
void draw_hkproc_popup(const std::string& procname, unsigned int rowNumber);

void draw_hooked_processes_list(bool* hookedProcessesList) {

	imgui::SetNextWindowSize({ 220.0f, 880.0f });

	if (!imgui::Begin(
		"Hooked Processes",
		hookedProcessesList
	)) {

		imgui::End();
	}
	else {

		g_dataController.check_active_procs();
		
		// Draw table of hooked processes
		static ImGuiTableFlags flags =
			ImGuiTableFlags_SizingFixedFit	|
			ImGuiTableFlags_BordersOuterH	|
			ImGuiTableFlags_BordersOuterV	|
			ImGuiTableFlags_ScrollY;

		if (imgui::BeginTable("processesTable", 1, flags)) {
			
			imgui::TableSetupColumn("processName", ImGuiTableColumnFlags_WidthFixed);
			
			for (unsigned int row = 0; row < g_dataController.get_hkprocs_size(); ++row) {
				
				static std::string	popupProcessName;
				static unsigned int popupRowNumber = 0;
				
				imgui::TableNextRow();
				imgui::TableSetColumnIndex(0);

				imgui::PushID(row);

				std::string procname = g_dataController.get_hkproc_name(row);

				imgui::Selectable(
					procname.c_str(),
					(row == g_dataController.get_selected_process()) ? true : false,
					ImGuiSelectableFlags_SpanAllColumns
				);

				if (imgui::IsItemHovered() && imgui::IsMouseClicked(0))
					g_dataController.set_selected_process(row);

				popupProcessName = procname;
				popupRowNumber = row;

				draw_hkproc_popup(popupProcessName, popupRowNumber);

				imgui::PopID();

			}

			imgui::EndTable();

		}

		imgui::End();

	}

}

void draw_hkproc_popup(const std::string& procname, unsigned int rowNumber) {

	imgui::SetNextWindowSize({ 200.0f, 80.0f });

	imgui::PushStyleColor(
		ImGuiCol_PopupBg,
		IM_COL32(
			50.0f,
			50.0f,
			50.0f,
			255.0f
		)
	);

	if (imgui::BeginPopupContextItem("hkProcessPopup")) {

		imgui::Text(procname.c_str());
		imgui::Separator();
		imgui::Separator();

		if (imgui::Selectable("Scan Memory", false)) {

			g_scanMemoryPopup = true;
			g_processToScan = rowNumber;
			g_dataController.set_selected_process(rowNumber);

			imgui::CloseCurrentPopup();

		}

		if (imgui::Selectable("Inject DLL", false)) {

			std::cout << rowNumber << '\n';

			imgui::CloseCurrentPopup();

		}

		imgui::EndPopup();

	}

	imgui::PopStyleColor();

}

#endif