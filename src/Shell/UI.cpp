#include "Shell/UI.hpp"
#include "Shell/Audio.hpp"
#include "Utilities/Randomizer.hpp"
#include "Utilities/Log.hpp"
#include "Utilities/Definitions.hpp"
#include <sstream>
#include <iomanip>

namespace Mines {
	UI::UI(tgui::Gui& gui, const std::string& ui_folder, const std::string& version) : 
		gui(gui),
		current_page(Page::MAIN_MENU) {
		try {
			loadPages(ui_folder);
			organizePages();
		} catch (const std::exception&) {
			handleError();
			execution_state = ExecutionState::FAILED;
			return;
		}
		
		setGameVersion(version);
		Audio::getInstance().playMusic("../../rc/Audio/Music/Welcome.ogg");
	};

	void UI::handleError() {
		gui.removeAllWidgets();
		auto* window = gui.getWindow();

		window->create(
			sf::VideoMode({ 500, 150 }),
			"Minesweeper - Error",
			sf::Style::Titlebar | sf::Style::Close
		);

		gui.setWindow(*window);
		auto label = tgui::Label::create(
			"Failed to load UI resources when creating the window\nPlease ensure the existence of the resources."
		);

		label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		label->setTextSize(16);
		gui.setFont(tgui::Font("../../rc/Fonts/ARIALUNI.TTF"));
		gui.add(label);
		
		float x = (500.f - label->getSize().x) / 2.f;
		float y = (150.f - label->getSize().y) / 2.f;
		label->setPosition({ x, y });
	}

	void UI::switchPage(Page page) {
		if (page == current_page) {
			return;
		}

		pages[current_page]->setVisible(false);
		pages[page]->setVisible(true);
		current_page = page;
	}

	void UI::setGameVersion(const std::string& version) {
		auto version_label = pages[Page::MAIN_MENU]->get<tgui::Label>("VersionLabel");
		version_label->setText("Version " + version);
	}

	void UI::setModeIndicator(int mode) {
		auto mode_label = pages[Page::GAME]->get<tgui::Label>("ControlModeLabel");

		if (mode == 0) {
			mode_label->setText("Control Mode: Move");
		} else {
			mode_label->setText("Control Mode: Mark");
		}
	}

	void UI::updateTimerDisplay(int total_seconds) {
		auto timer_label = pages[Page::GAME]->get<tgui::Label>("TimerLabel");
		int minutes = total_seconds / 60;
		int seconds = total_seconds % 60;
		std::stringstream ss;

		ss << std::setw(2) << std::setfill('0') << minutes << ":"
		   << std::setw(2) << std::setfill('0') << seconds;
		timer_label->setText(ss.str());
	}

	void UI::updateGamePage(const Map& map, const Player& player) {
		auto player_name_label = pages[Page::GAME]->get<tgui::Label>("PlayerNameLabel");
		auto step_label = pages[Page::GAME]->get<tgui::Label>("StepLabel");
		auto mark_count_label = pages[Page::GAME]->get<tgui::Label>("MarkCountLabel");
		auto remaining_label = pages[Page::GAME]->get<tgui::Label>("RemainingLabel");

		player_name_label->setText("Player Name: " + player.getName());
		step_label->setText("Step: " + std::to_string(player.getStep()));
		mark_count_label->setText("Mark count: " + std::to_string(player.getMarkCount()));
		remaining_label->setText("Remaining: " + std::to_string(map.getCellToGo()));

		for (int r = 0; r < 16; ++r) {
			for (int c = 0; c < 16; ++c) {
				const auto& cell = map.grid[r][c];
				auto& label = cell_labels[r * 16 + c];
				auto renderer = label->getRenderer();

				if (!cell.is_revealed) {
					label->setText("?");
					renderer->setTextColor(tgui::Color::Black);
					renderer->setBackgroundColor(tgui::Color(211, 211, 211));
				} else {
					renderer->setBackgroundColor(tgui::Color::Green);

					if (cell.is_mine) {
						label->setText("X");
						renderer->setTextColor(tgui::Color::Red);
					} else {
						label->setText(std::to_string(cell.mines_around));
						renderer->setTextColor(tgui::Color::Black);
					}
				}

				if (cell.is_marked) {
					renderer->setBackgroundColor(tgui::Color::Yellow);
				}

				if (cell.player_here) {
					renderer->setBorderColor(tgui::Color::Blue);
					renderer->setBorders(3);
				} else {
					renderer->setBorderColor(tgui::Color::Black);
					renderer->setBorders(1);
				}
			}
		}
	}

	void UI::setResultPage(
		const Player& player,
		const Map& map,
		const std::string& result,
		int total_seconds,
		int df_index
	) {
		auto player_name_label = pages[Page::RESULT]->get<tgui::Label>("PlayerNameLabel");
		auto difficulty_label = pages[Page::RESULT]->get<tgui::Label>("DifficultyLabel");
		auto result_label = pages[Page::RESULT]->get<tgui::Label>("ResultLabel");
		auto step_label = pages[Page::RESULT]->get<tgui::Label>("StepLabel");
		auto remaining_label = pages[Page::RESULT]->get<tgui::Label>("RemainingLabel");
		auto mark_count_label = pages[Page::RESULT]->get<tgui::Label>("MarkCountLabel");
		auto total_time_label = pages[Page::RESULT]->get<tgui::Label>("TotalTimeLabel");

		int minutes = total_seconds / 60;
		int seconds = total_seconds % 60;
		std::stringstream total_time;
		total_time << minutes << "m " << seconds << 's';
		std::string difficulty;

		if (df_index == 0) {
			difficulty = "Easy";
		} else if (df_index == 1) {
			difficulty = "Medium";
		} else {
			difficulty = "Hard";
		}

		player_name_label->setText("Player Name: " + player.getName());
		difficulty_label->setText("Difficulty: " + difficulty);
		result_label->setText("Result: " + result);
		step_label->setText("Step: " + std::to_string(player.getStep()));
		remaining_label->setText("Remaining: " + std::to_string(map.getCellToGo()));
		mark_count_label->setText("Mark Count: " + std::to_string(player.getMarkCount()));
		total_time_label->setText("Total Time: " + total_time.str());
	}

	void UI::fitBackground(tgui::Picture::Ptr background) {
		if (!background) {
			return;
		}

		float scale = 1080.f / background->getSize().y;
		float new_w = background->getSize().x * scale;
		float new_h = background->getSize().y * scale;
		background->setSize({ new_w, new_h });

		float offset_x = (new_w - 1920.f) / 2.f;
		float offset_y = (new_h - 1080.f) / 2.f;
		background->setPosition({ -offset_x, -offset_y });
	}

	void UI::loadPages(const std::string& folder) {
		pages[Page::MAIN_MENU] = tgui::Group::create();
		pages[Page::SETTINGS] = tgui::Group::create();
		pages[Page::OPTIONS] = tgui::Group::create();
		pages[Page::GAME] = tgui::Group::create();
		pages[Page::PAUSE] = tgui::Group::create();
		pages[Page::RESULT] = tgui::Group::create();

		try {
			pages[Page::MAIN_MENU]->loadWidgetsFromFile(folder + "MainMenu.txt");
			pages[Page::SETTINGS]->loadWidgetsFromFile(folder + "Settings.txt");
			pages[Page::OPTIONS]->loadWidgetsFromFile(folder + "Options.txt");
			pages[Page::GAME]->loadWidgetsFromFile(folder + "GamePage.txt");
			pages[Page::PAUSE]->loadWidgetsFromFile(folder + "Pause.txt");
			pages[Page::RESULT]->loadWidgetsFromFile(folder + "Result.txt");
		} catch (const tgui::Exception& e) {
			LOG_ERROR("Failed to load UI pages: {}", e.what());
			throw;
		}

		pages[Page::MAIN_MENU]->loadWidgetsFromFile(folder + "MainMenu.txt");
		pages[Page::SETTINGS]->loadWidgetsFromFile(folder + "Settings.txt");
		pages[Page::OPTIONS]->loadWidgetsFromFile(folder + "Options.txt");
		pages[Page::GAME]->loadWidgetsFromFile(folder + "GamePage.txt");
		pages[Page::PAUSE]->loadWidgetsFromFile(folder + "Pause.txt");
		pages[Page::RESULT]->loadWidgetsFromFile(folder + "Result.txt");

		fitBackground(pages[Page::MAIN_MENU]->get<tgui::Picture>("Background"));
		fitBackground(pages[Page::SETTINGS]->get<tgui::Picture>("Background"));
		fitBackground(pages[Page::OPTIONS]->get<tgui::Picture>("Background"));
		fitBackground(pages[Page::GAME]->get<tgui::Picture>("Background"));
		fitBackground(pages[Page::PAUSE]->get<tgui::Picture>("Background"));
		fitBackground(pages[Page::RESULT]->get<tgui::Picture>("Background"));

		pages[Page::SETTINGS]->setVisible(false);
		pages[Page::OPTIONS]->setVisible(false);
		pages[Page::GAME]->setVisible(false);
		pages[Page::PAUSE]->setVisible(false);
		pages[Page::RESULT]->setVisible(false);

		gui.add(pages[Page::MAIN_MENU]);
		gui.add(pages[Page::SETTINGS]);
		gui.add(pages[Page::OPTIONS]);
		gui.add(pages[Page::GAME]);
		gui.add(pages[Page::PAUSE]);
		gui.add(pages[Page::RESULT]);
	}

	void UI::organizePages() {
		try {
			auto play_button = pages[Page::MAIN_MENU]->get<tgui::Button>("PlayButton");
			auto settings_button = pages[Page::MAIN_MENU]->get<tgui::Button>("SettingsButton");
			auto quit_button = pages[Page::MAIN_MENU]->get<tgui::Button>("QuitButton");

			play_button->onPress(
				[this]() {
					LOG_INFO("Play button clicked.");
					Audio::getInstance().playSound("Click", true);
					switchPage(Page::OPTIONS);
				}
			);

			settings_button->onPress(
				[this]() {
					LOG_INFO("Settings button clicked.");
					Audio::getInstance().playSound("Click", true);
					switchPage(Page::SETTINGS);
				}
			);

			quit_button->onPress(
				[this]() {
					LOG_INFO("Quit button clicked.");
					Audio::getInstance().playSound("Click", true);
					Audio::getInstance().stopMusic();
					gui.getWindow()->close();
				}
			);
		} catch (const tgui::Exception& e) {
			LOG_ERROR("Failed to bind MainMenu UI callbacks: {}", e.what());
			throw;
		}

		try {
			auto save_button = pages[Page::SETTINGS]->get<tgui::Button>("SaveButton");
			auto music_volume_slider = pages[Page::SETTINGS]->get<tgui::Slider>("MusicVolumeSlider");
			auto sound_volume_slider = pages[Page::SETTINGS]->get<tgui::Slider>("SoundVolumeSlider");

			save_button->onPress(
				[this]() {
					LOG_INFO("Save button clicked.");
					Audio::getInstance().playSound("Click", true);
					switchPage(Page::MAIN_MENU);
				}
			);

			music_volume_slider->onValueChange(
				[this](float value) {
					LOG_INFO("Music volume changed to {}.", value);
					Audio::getInstance().setMusicVolume(value);
				}
			);

			sound_volume_slider->onValueChange(
				[this](float value) {
					LOG_INFO("Sound volume changed to {}.", value);
					Audio::getInstance().setSoundVolume(value);
				}
			);
		} catch (const tgui::Exception& e) {
			LOG_ERROR("Failed to bind Settings UI callbacks: {}", e.what());
			throw;
		}

		try {
			auto back_button = pages[Page::OPTIONS]->get<tgui::Button>("BackButton");
			auto ready_button = pages[Page::OPTIONS]->get<tgui::Button>("ReadyButton");

			back_button->onPress(
				[this]() {
					LOG_INFO("Back button clicked.");
					Audio::getInstance().playSound("Click", true);
					switchPage(Page::MAIN_MENU);
				}
			);

			ready_button->onPress(
				[this]() {
					LOG_INFO("Ready button clicked.");
					Audio::getInstance().playSound("Click", true);
					
					auto name_input = pages[Page::OPTIONS]->get<tgui::EditBox>("YourNameEditBox");
					auto df_dropdown = pages[Page::OPTIONS]->get<tgui::ComboBox>("DifficultyComboBox");
					auto start_x_input = pages[Page::OPTIONS]->get<tgui::EditBox>("StartPosXEditBox");
					auto start_y_input = pages[Page::OPTIONS]->get<tgui::EditBox>("StartPosYEditBox");

					std::string player_name = name_input->getText().toStdString();
					int df_index = df_dropdown->getSelectedItemIndex();
					int start_x = start_x_input->getText().toInt(Randomizer::getInstance().getInt(0, 15));
					int start_y = start_y_input->getText().toInt(Randomizer::getInstance().getInt(0, 15));
					adapter.start_game(player_name, start_x, start_y, df_index);

					switchPage(Page::GAME);

					Audio::getInstance().playMusic(
						"../../rc/Audio/Music/LuckyOnTheBrink.ogg",
						0.0f, true, 23.177f, 246.76f
					);
				}
			);
		} catch (const tgui::Exception& e) {
			LOG_ERROR("Failed to bind Options UI callbacks: {}", e.what());
			throw;
		}

		try {
			auto grid = tgui::Grid::create();
			grid->setSize({ 800, 800 });
			grid->setPosition({ 560, 50 });
			pages[Page::GAME]->add(grid);
			cell_labels.resize(16 * 16);

			for (int row = 0; row < 16; ++row) {
				for (int col = 0; col < 16; ++col) {
					auto label = tgui::Label::create();
					auto renderer = label->getRenderer();
					label->setSize({ 50, 50 });
					label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
					label->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
					label->setText("");
					label->setTextSize(24);
					renderer->setBackgroundColor(tgui::Color(211, 211, 211));
					renderer->setTextColor(tgui::Color::Black);
					renderer->setBorders(1);
					renderer->setBorderColor(tgui::Color::Black);
					grid->addWidget(label, row, col);
					cell_labels[row * 16 + col] = label;
				}
			}
		} catch (const tgui::Exception& e) {
			LOG_ERROR("Failed to bind GamePage UI callbacks: {}", e.what());
			throw;
		}

		try {
			auto resume_button = pages[Page::PAUSE]->get<tgui::Button>("ResumeButton");
			auto give_up_button = pages[Page::PAUSE]->get<tgui::Button>("GiveUpButton");
			auto exit_button = pages[Page::PAUSE]->get<tgui::Button>("ExitButton");

			resume_button->onPress(
				[this]() {
					LOG_INFO("Resume button clicked.");
					Audio::getInstance().playSound("Click", true);
					switchPage(Page::GAME);
					adapter.resume_game();
				}
			);

			give_up_button->onPress(
				[this]() {
					LOG_INFO("Give Up button clicked.");
					Audio::getInstance().playSound("Click", true);
					switchPage(Page::GAME);
					adapter.stop_game("Lose");
				}
			);

			exit_button->onPress(
				[this]() {
					LOG_INFO("Exit button clicked.");
					Audio::getInstance().playSound("Click", true);
					switchPage(Page::MAIN_MENU);
					Audio::getInstance().playMusic("../../rc/Audio/Music/Welcome.ogg");
				}
			);
		} catch (const tgui::Exception& e) {
			LOG_ERROR("Failed to bind Pause UI callbacks: {}", e.what());
			throw;
		}

		try {
			auto ok_button = pages[Page::RESULT]->get<tgui::Button>("OKButton");

			ok_button->onPress(
				[this]() {
					LOG_INFO("OK button clicked.");
					Audio::getInstance().playSound("Click", true);
					switchPage(Page::MAIN_MENU);
					Audio::getInstance().playMusic("../../rc/Audio/Music/Welcome.ogg");
				}
			);
		} catch (const tgui::Exception& e) {
			LOG_ERROR("Failed to bind Result UI callbacks: {}", e.what());
			throw;
		}
	}
}
