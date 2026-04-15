#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <string>
#include <memory>
#include <cstdlib>
#include <filesystem>
#include "Shell/UI.hpp"
#include "Shell/Audio.hpp"
#include "Core/System.hpp"
#include "Core/Player.hpp"
#include "Core/Map.hpp"
#include "Core/List.hpp"
#include "Utilities/Log.hpp"
#include "Utilities/Definitions.hpp"

namespace fs = std::filesystem;

namespace Mines {
	static void printTexts(const std::string& path) {
		List texts;
		texts.load(path);
		size_t size = texts.getSize();
		
		for (size_t i = 0; i < size; ++i) {
			std::cout << texts.get(i, "") << '\n';
		}
	}

	static bool checkAndRestoreData(const std::string& path) {
		fs::path root = fs::absolute(path).parent_path().parent_path().parent_path();
		fs::path target = root / "data" / "Config" / "Settings.cfg";
		fs::path origin = root / "rc" / "Backups" / "Settings.cfg";

		if (!fs::exists(target)) {
			try {
				fs::create_directories(target.parent_path());
				fs::copy_file(origin, target);
				LOG_INFO("Restored: {}", target.string());
			} catch (const fs::filesystem_error& e) {
				LOG_ERROR("Filesystem Error: {}", e.what());
				return false;
			}
		}

		return true;
	}

	static void processArguments(const std::vector<std::string>& args) {
		if (args.size() == 1) {
			return;
		}

		for (int i = 0; i < args.size(); ++i) {
			if (args[i] == "/debug") {
#ifdef NDEBUG				
#ifdef _WIN32
				std::system("start ../Debug/Minesweeper.exe");
				break;
#else
				std::system("../Debug/Minesweeper &");
				break;
#endif // _WIN32
#else
				return;
#endif // NDEBUG
			} else if (args[i] == "/restore_data") {
				if (!checkAndRestoreData(args[0])) {
					execution_state = ExecutionState::FAILED;
				}

				break;
			} else if (args[i] == "--information" || args[i] == "-info") {
				printTexts("../../rc/Texts/Info.txt");
			} else if (args[i] == "--help" || args[i] == "-?") {
				printTexts("../../rc/Texts/Help.txt");
			}
		}

		std::exit(static_cast<int>(execution_state));
	}
} // namespace Mines

int main(int argc, char* argv[]) {
	Mines::processArguments(std::vector<std::string>(argv, argv + argc));
	Mines::checkAndRestoreData(std::string(argv[0]));

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

	ui.bridge.start_game = [&](const std::string& name, int x, int y, int df_index) {
		system.startGame(name, x, y, df_index, player, map, ui);
	};

	ui.bridge.resume_game = [&]() {
		system.resumeGame();
	};

	ui.bridge.stop_game = [&](const std::string& res) {
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
