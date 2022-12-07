#ifndef LOGWND
#define LOGWND

void draw_log_window(bool* logWindow) {

	imgui::SetNextWindowSize({ 570.0f, 300.0f });

	if (!imgui::Begin(
		"Log",
		logWindow
	)) {

		imgui::End();
	}
	else {

		static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY;

		if (imgui::BeginTable("logTable", 1, flags)) {

			imgui::TableSetupColumn("log", ImGuiTableColumnFlags_WidthFixed);

			unsigned int size = static_cast<int>(g_logger.get_logs().size());

			ImGuiListClipper clipper;
			clipper.Begin(size);

			while (clipper.Step()) {

				for (unsigned int row = clipper.DisplayStart; row < static_cast<unsigned int>(clipper.DisplayEnd); ++row) {

					imgui::TableNextRow();
					imgui::TableSetColumnIndex(0);

					imgui::PushID(row);

					imgui::TextColored(
						g_logger.get_logs().at(row).second, 
						g_logger.get_logs().at(row).first.c_str()
					);

					imgui::PopID();

				}

			}

			clipper.End();
			imgui::EndTable();

		}

		imgui::End();

	}

}

#endif