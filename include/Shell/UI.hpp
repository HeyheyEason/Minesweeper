#pragma once

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include "Utilities/Message.hpp"
#include "Core/Map.hpp"
#include "Core/Player.hpp"
#include "Shell/Audio.hpp"
#include "Shell/Keyboard.hpp"

namespace Mines {
	enum class Page {
		MAIN_MENU,
		SETTINGS,
		KEYBOARD,
		OPTIONS,
		GAME,
		PAUSE,
		RESULT
	};

	class UI {
	public:
		explicit UI(tgui::Gui& gui, const std::string& ui_folder, const std::string& version);
		void handleError();
		void switchPage(Page page);
		void setGameVersion(const std::string& version);
		void updateKeyboardDisplay(const std::string& action, const std::string& key);
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

		Keyboard& getKeyboard();
		Message pollMessage();
		void handleInput(const sf::Event& event, bool is_playing);

	private:
		tgui::Gui& gui;
		std::map<Page, tgui::Group::Ptr> pages;
		Page current_page;
		std::vector<tgui::Label::Ptr> cell_labels;

		Keyboard keyboard;
		std::queue<Message> message_queue;

		void loadPages(const std::string& folder);
		void organizePages();
	};
} // namespace Mines
