#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <string>
#include <memory>
#include "Shell/UI.hpp"
#include "Shell/Audio.hpp"
#include "Core/System.hpp"
#include "Core/Player.hpp"
#include "Core/Map.hpp"
#include "Utilities/Log.hpp"
#include "Utilities/Definitions.hpp"

int main() {
	sf::RenderWindow window(sf::VideoMode({ 1920, 1000 }), "Minesweeper");
	tgui::Gui gui(window);

	auto& audio = Mines::Audio::getInstance();
	audio.loadSounds("../../rc/Audio/Sounds/");
	Mines::UI ui(gui, "../../rc/Forms/", "1.2.1");
	Mines::System system;
	Mines::Player player("Player", 0, 0);
	Mines::Map map;

	if (sf::Image icon; icon.loadFromFile("../../rc/Icon/RuntimeIcon.png")) {
		window.setIcon(icon);
	}

	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);
	LOG_INFO("Testing...");

	ui.adapter.start_game = [&](const std::string& name, int x, int y, int df_index) {
		system.startGame(name, x, y, df_index, player, map, ui);
	};

	ui.adapter.resume_game = [&]() {
		system.resumeGame();
	};

	ui.adapter.stop_game = [&](const std::string& res) {
		system.stopGame(res, player, map, ui);
	};

	while (window.isOpen()) {
		while (auto event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}

			gui.handleEvent(*event);

			if (system.isPlaying()) {
				system.handleInput(*event, player, map, ui);
			}
		}

		if (system.isPlaying()) {
			system.updateTimer(ui);
		}

		audio.cleanup();
		window.clear(sf::Color::White);
		gui.draw();
		window.display();
	}

	LOG_INFO("Test completed with exit code {}.", Mines::execution_state);
	return static_cast<int>(Mines::execution_state);
}
