#ifndef MEMVIEWER
#define MEMVIEWER

unsigned int	memActiveRow = 0;
bool			isMemSelectionActive = false;

void draw_memory_viewer(bool* memoryViewer);
void draw_memory_popup(const std::string& memaddrDec, unsigned int rowNumber);

void draw_memory_viewer(bool* memoryViewer) {
	
	static unsigned int lastSelectedProcess = g_dataController.get_selected_process();

	if (lastSelectedProcess != g_dataController.get_selected_process()) {

		lastSelectedProcess = g_dataController.get_selected_process();
		memActiveRow = 0;
		isMemSelectionActive = false;

	}

	imgui::SetNextWindowSize({ 900.0f, 550.0f });

	if (!imgui::Begin(
		"Memory",
		memoryViewer
	)) {

		imgui::End();
	}
	else {

		static ImGuiTableFlags flags =
			ImGuiTableFlags_SizingFixedFit	|
			ImGuiTableFlags_Borders			|
			ImGuiTableFlags_Resizable		|
			ImGuiTableFlags_Reorderable		|
			ImGuiTableFlags_ScrollY			|
			ImGuiTableFlags_Hideable;

		static unsigned int numberOfColumns = 6;

		if (imgui::BeginTable("memViewer", numberOfColumns, flags)) {

			imgui::TableSetupColumn("Row",				ImGuiTableColumnFlags_WidthFixed);
			imgui::TableSetupColumn("Address Hex",		ImGuiTableColumnFlags_WidthFixed);
			imgui::TableSetupColumn("Address Decimal",	ImGuiTableColumnFlags_WidthFixed);
			imgui::TableSetupColumn("Value Decimal",	ImGuiTableColumnFlags_WidthStretch);
			imgui::TableSetupColumn("Value Hex",		ImGuiTableColumnFlags_WidthStretch);
			imgui::TableSetupColumn("String",			ImGuiTableColumnFlags_WidthStretch);

			imgui::TableSetupScrollFreeze(0, 1);
			imgui::TableHeadersRow();

			if (g_dataController.is_hkprocs_empty()) {

				imgui::EndTable();
				imgui::End();

				return;
			}

			unsigned int size = static_cast<int>(g_dataController.hkproc_get_size_of_scanned_ints());

			ImGuiListClipper clipper;
			clipper.Begin(size);

			while (clipper.Step()) {

				for (unsigned int row = clipper.DisplayStart; row < static_cast<unsigned int>(clipper.DisplayEnd); ++row) {

					imgui::TableNextRow();
					imgui::PushID(row);

					long long	memaddr = g_dataController.hkproc_get_scanned_int_address(row);
					int			memval = g_dataController.hkproc_get_scanned_int_value(row);

					std::string memaddrStr = std::to_string(memaddr);
					
					for (unsigned int column = 0; column < numberOfColumns; ++column) {

						imgui::TableSetColumnIndex(column);

						switch (column) {

							case 0: {

								imgui::Selectable(
									std::to_string(row).c_str(), 
									(memActiveRow == row && isMemSelectionActive == true) ? true : false,
									ImGuiSelectableFlags_SpanAllColumns
								);

								draw_memory_popup(memaddrStr, row);

								break;
							}

							case 1:
								imgui::Text("0x%p", memaddr);
								break;

							case 2:
								imgui::Text(memaddrStr.c_str());
								break;

							case 3:
								imgui::Text("%d", memval);
								break;

							case 4:
								imgui::Text("%0x", memval);
								break;

							case 5:
								imgui::Text(decimal_to_string(memval).c_str());
								break;
							
						}

					}

					imgui::PopID();

				}

			}

			clipper.End();
			imgui::EndTable();

		}

		imgui::End();

	}

}

void draw_memory_popup(const std::string& memaddrDec, unsigned int rowNumber) {

	imgui::SetNextWindowSize({ 200.0f, 145.0f });

	imgui::PushStyleColor(
		ImGuiCol_PopupBg,
		IM_COL32(
			50.0f,
			50.0f,
			50.0f,
			255.0f
		)
	);

	if (imgui::BeginPopupContextItem("memoryPopup")) {

		memActiveRow = rowNumber;
		isMemSelectionActive = true;

		imgui::Text("0x%p", std::stoll(memaddrDec));
		imgui::Separator();
		imgui::Separator();

		if (imgui::Selectable("Watch this address", false)) {

			g_dataController.hkproc_add_int_to_watch(rowNumber);

			isMemSelectionActive = false;
			imgui::CloseCurrentPopup();
			
		}

		if (imgui::Selectable("Watch all addresses", false)) {

			g_dataController.watch_all_scanned_addresses();

			isMemSelectionActive = false;
			imgui::CloseCurrentPopup();
			
		}

		imgui::Separator();

		if (imgui::Selectable("Delete this row", false)) {

			isMemSelectionActive = false;
			imgui::CloseCurrentPopup();

		}

		if (imgui::Selectable("Delete all but this", false)) {

			isMemSelectionActive = false;
			imgui::CloseCurrentPopup();

		}

		if (imgui::Selectable("Delete all", false)) {

			isMemSelectionActive = false;
			imgui::CloseCurrentPopup();

		}

		imgui::EndPopup();

	}
		
	imgui::PopStyleColor();

}

#endif