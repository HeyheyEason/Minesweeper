#pragma once

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <map>
#include <vector>
#include <string>
#include <functional>
#include "Utilities/Adapter.hpp"
#include "Core/Map.hpp"
#include "Core/Player.hpp"
#include "Shell/Audio.hpp"

namespace Mines {
	enum class Page {
		MAIN_MENU,
		SETTINGS,
		OPTIONS,
		GAME,
		PAUSE,
		RESULT
	};

	class UI {
	public:
		Adapter adapter;
		explicit UI(tgui::Gui& gui, const std::string& ui_folder, const std::string& version);
		void handleError();
		void switchPage(Page page);
		void setGameVersion(const std::string& version);
		void setModeIndicator(int mode);
		void updateTimerDisplay(int total_seconds);
		void updateGamePage(const Map& map, const Player& player);

		void setResultPage(
			const Player& player,
			const Map& map,
			const std::string& result,
			int total_seconds,
			int df_index
		);

	private:
		tgui::Gui& gui;
		std::map<Page, tgui::Group::Ptr> pages;
		Page current_page;
		std::vector<tgui::Label::Ptr> cell_labels;

		void fitBackground(tgui::Picture::Ptr background);
		void loadPages(const std::string& folder);
		void organizePages();
	};
} // namespace Mines
