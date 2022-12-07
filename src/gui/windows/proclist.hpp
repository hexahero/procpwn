#ifndef PROCLIST
#define PROCLIST

unsigned int	procActiveRow = 0;
bool			isProcSelectionActive = false;

void draw_processes_list(bool* processesList);
void draw_proc_popup(const std::string& procname, unsigned int rowNumber);
void draw_too_many_hooked_procs_popup(bool* tooManyHProcsPopup);

void draw_processes_list(bool* processesList) {

	imgui::SetNextWindowSize({ 380.0f, 450.0f });

	if (!imgui::Begin(
		"Processes",
		processesList
	)) {

		imgui::End();
	}
	else {

		// Constantly refresh processes list to sort out those that are exited.
		// As far as we know that is the cheapest way to check if the process is still active. (i.e. without getting a handle)
		// However this approach still has a significant performance overhead and something has to be done with it. Probably.
		// Notice that putting this check in another thread will cause race condition when drawing gui table. (see below)
		if (!g_dataController.is_procs_empty())
			obtain_processes_from_system(g_dataController.get_procs());

		// Draw table of all running processes
		static ImGuiTableFlags flags =
			ImGuiTableFlags_SizingFixedFit	|
			ImGuiTableFlags_RowBg			|
			ImGuiTableFlags_Borders			|
			ImGuiTableFlags_Resizable		|
			ImGuiTableFlags_Reorderable		|
			ImGuiTableFlags_ScrollY			|
			ImGuiTableFlags_Hideable;

		if (imgui::BeginTable("processesTable", 3, flags)) {

			static std::string	popupProcessName;
			static unsigned int popupRowNumber;
			
			imgui::TableSetupColumn("Name",		ImGuiTableColumnFlags_WidthFixed);
			imgui::TableSetupColumn("ID",		ImGuiTableColumnFlags_WidthStretch);
			imgui::TableSetupColumn("ParentID",	ImGuiTableColumnFlags_WidthStretch);

			imgui::TableSetupScrollFreeze(0, 1);
			imgui::TableHeadersRow();
			
			for (unsigned int row = 0; row < g_dataController.get_procs_size(); ++row) {

				imgui::TableNextRow();
				imgui::PushID(row);

				for (int column = 0; column < 3; ++column) {

					imgui::TableSetColumnIndex(column);

					switch (column) {

						case 0: {

							std::string procname = g_dataController.get_proc_name(row);

							imgui::Selectable(
								procname.c_str(), 
								(procActiveRow == row && isProcSelectionActive == true) ? true : false,
								ImGuiSelectableFlags_SpanAllColumns
							);

							if (imgui::IsItemHovered() && imgui::IsMouseClicked(0)) {

								procActiveRow = row;
								isProcSelectionActive = true;

							}
								
							popupProcessName = procname;
							popupRowNumber = row;

							draw_proc_popup(popupProcessName, popupRowNumber);

							break;
						}
						
						case 1:
							imgui::Text("%d", static_cast<int>(g_dataController.get_proc_id(row)));
							break;

						case 2:
							imgui::Text("%d", static_cast<int>(g_dataController.get_proc_pid(row)));
							break;

					}
					
				}

				imgui::PopID();

			}

			imgui::EndTable();

		}

		imgui::End();

	}

}

void draw_proc_popup(const std::string& procname, unsigned int rowNumber) {

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

	if (imgui::BeginPopupContextItem("processPopup")) {

		procActiveRow = rowNumber;
		isProcSelectionActive = true;

		imgui::Text(procname.c_str());
		imgui::Separator();
		imgui::Separator();

		if (imgui::Selectable("Hook Process", false)) {

			// Check if process with this name and id is already hooked
			bool alreadyHooked = g_dataController.is_already_hooked(rowNumber);

			if (bool tooManyHooks = (g_dataController.get_hkprocs_size() != MAX_PROCESSES) && !alreadyHooked) {

				if (g_dataController.hook_process(procname, rowNumber))
					g_logger.add_log(procname + ": process successfully hooked", LOGSUCCESS);
				else
					g_logger.add_log("failed to hook process", LOGFAILURE);

			}
			else {

				if (alreadyHooked)
					g_logger.add_log("each process can be hooked only once", LOGFAILURE);
				else if (tooManyHooks)
					g_tooManyHkProcsPopup = true;

				g_logger.add_log("failed to hook process", LOGFAILURE);

			}
				
			isProcSelectionActive = false;
			imgui::CloseCurrentPopup();

		}

		if (imgui::Selectable("Kill Process", false)) {

			auto procHandle = get_process_handle(procname);

			if (procHandle != NULL && kill_process(procHandle)) {

				g_logger.add_log(procname + " killed", LOGSUCCESS);
				std::cout << procname << " killed\n";

			}
			else {

				g_logger.add_log("could not kill process", LOGFAILURE);
				std::cout << "could not kill process\n";

			}
				
			isProcSelectionActive = false;
			imgui::CloseCurrentPopup();

		}

		imgui::EndPopup();

	}

	imgui::PopStyleColor();

}

#endif