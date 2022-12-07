#ifndef WATCHVIEW
#define WATCHVIEW

#include <format>
#include <cctype>

void draw_watch_view(bool* watchView);
void delete_inaccessible_watch_row(std::vector<unsigned int>& inaccessibleWatchedMemIndices);
//draw_watch_popup();

void draw_watch_view(bool* watchView) {

	imgui::SetNextWindowSize({ 900.0f, 350.0f });

	if (!imgui::Begin(
		"Watch",
		watchView
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

		if (imgui::BeginTable("watchView", numberOfColumns, flags)) {
			
			static std::vector<unsigned int> inaccessibleWatchedMemIndices;
			
			// Setup table rows
			imgui::TableSetupColumn("Row",				ImGuiTableColumnFlags_WidthFixed);
			imgui::TableSetupColumn("Address Hex",		ImGuiTableColumnFlags_WidthFixed);
			imgui::TableSetupColumn("Address Decimal",	ImGuiTableColumnFlags_WidthFixed);
			imgui::TableSetupColumn("Value Decimal",	ImGuiTableColumnFlags_WidthStretch);
			imgui::TableSetupColumn("Value Hex",		ImGuiTableColumnFlags_WidthStretch);
			imgui::TableSetupColumn("String",			ImGuiTableColumnFlags_WidthStretch);

			// Setup table header
			imgui::TableSetupScrollFreeze(0, 1);
			imgui::TableHeadersRow();

			if (g_dataController.is_hkprocs_empty()) {

				imgui::EndTable();
				imgui::End();

				return;
			}
			g_dataController.hkproc_get_size_of_watched_ints();
			unsigned int size = static_cast<int>(g_dataController.hkproc_get_size_of_watched_ints());

			// Setup table clipper
			ImGuiListClipper clipper;
			clipper.Begin(size);

			while (clipper.Step()) {

				// For each row
				for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row) {

					imgui::TableNextRow();
					imgui::PushID(row);

					long long	memaddr = g_dataController.hkproc_get_watched_int_address(row);
					std::string memaddrDec = std::to_string(g_dataController.hkproc_get_watched_int_value(row));
					int			memval = 0;

					// Peek victim process memory address
					bool readResult = read_process_memory(
						g_dataController.get_hkproc_handle(),
						memaddr,
						&memval,
						sizeof(memval)
					);

					// Push element for deletion and create log if read failed
					if (!readResult) {
						
						std::string procName = g_dataController.get_hkproc_name();
						std::string addrHexStr = std::format("{:x}", memaddr);

						for (auto& character : addrHexStr)
							character = static_cast<char>(std::toupper(character));

						g_logger.add_log(procName + ": " + "error reading process memory at [0x" + addrHexStr + "]", LOGWARNING);

						inaccessibleWatchedMemIndices.push_back(row);

					}

					// Set peeked value
					g_dataController.hkprocs_at()._watchedInts.at(row).second = memval;

					// For each column
					for (unsigned int column = 0; column < numberOfColumns; column++) {

						imgui::TableSetColumnIndex(column);

						switch (column) {

							case 0:
								imgui::Selectable(std::to_string(row).c_str(), false, ImGuiSelectableFlags_SpanAllColumns);
								break;

							case 1:
								imgui::Text("0x%p", memaddr);
								break;

							case 2:
								imgui::Text(memaddrDec.c_str());
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

					// Delete watched memory table row containing inaccessible watched memory
					delete_inaccessible_watch_row(inaccessibleWatchedMemIndices);

				}

			}

			clipper.End();
			imgui::EndTable();

		}

		imgui::End();

	}

}

void delete_inaccessible_watch_row(std::vector<unsigned int>& inaccessibleWatchedMemIndices) {

	for (const auto& indx : inaccessibleWatchedMemIndices) {

		g_dataController.hkprocs_at()._watchedInts.erase(
			g_dataController.hkprocs_at()._watchedInts.begin() + indx
		);

	}

	inaccessibleWatchedMemIndices.clear();

}

#endif