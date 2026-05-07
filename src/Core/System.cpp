#include "Core/System.hpp"

namespace Mines {
	void System::startGame(
		const std::string& name,
		int x,
		int y,
		int df_index,
		Player& player,
		Map& map,
		UI& ui
	) {
		playing = true;
		clock.restart();
		player = Player(name.empty() ? "Player" : name, x, y);
		map = Map();
		current_mode = ControlMode::MOVE;
		difficulty = df_index;
		std::set<int> safe_cells;

		for (int i = x - 1; i <= x + 1; ++i) {
			for (int j = y - 1; j <= y + 1; ++j) {
				if (i >= 0 && i < 16 && j >= 0 && j < 16) {
					safe_cells.insert(i * 16 + j);
				}
			}
		}

		map.init(safe_cells, static_cast<Difficulty>(9 * difficulty + 36));
		map.grid[x][y].player_here = true;
		map.processCombo(x, y);
		ui.setModeIndicator(static_cast<int>(current_mode));
		ui.updateGamePage(map, player);
	}

	void System::resumeGame() {
		playing = true;
		clock.start();
	}

	void System::stopGame(
		const std::string& result,
		const Player& player,
		Map& map,
		UI& ui
	) {
		playing = false;
		clock.stop();
		int total_seconds = static_cast<int>(clock.getElapsedTime().asSeconds());

		if (result == "Win") {
			map.revealAllCells(true);
			Audio::getInstance().playSound("Clear");
		} else {
			Audio::getInstance().playSound("Death");
		}

		ui.updateGamePage(map, player);
		ui.setResultPage(player, map, result, total_seconds, difficulty);
		Audio::getInstance().playMusic("../../rc/Audio/Music/LuckyOnTheBrink.ogg", 244.57f, false);

		tgui::Timer::scheduleCallback(
			[&ui]() {
				ui.switchPage(Page::RESULT);
				Audio::getInstance().playMusic("../../rc/Audio/Music/BackHome.ogg");
			}, 
			sf::seconds(3.f)
		);
	}

	void System::handleInput(
		const sf::Event& event,
		Player& player,
		Map& map,
		UI& ui
	) {
		if (!playing) {
			return;
		}

		if (const auto* key_pressed = event.getIf<sf::Event::KeyPressed>()) {
			if (key_pressed->scancode == keyboard.getKey("Pause")) {
				playing = false;
				clock.stop();
				Audio::getInstance().playSound("Click", true);
				ui.switchPage(Page::PAUSE);
				return;
			}

			if (key_pressed->scancode == keyboard.getKey("SwitchMode")) {
				current_mode = (current_mode == ControlMode::MOVE) ? ControlMode::MARK : ControlMode::MOVE;
				Audio::getInstance().playSound("Switch");
				ui.setModeIndicator(static_cast<int>(current_mode));
				return;
			}

			auto [r, c] = player.getPosition();
			int target_r = r;
			int target_c = c;
			MoveDirection dir;

			if (auto key = key_pressed->scancode; key == keyboard.getKey("MoveUp")) {
				--target_r;
				dir = MoveDirection::UP;
			} else if (key == keyboard.getKey("MoveDown")) {
				++target_r;
				dir = MoveDirection::DOWN;
			} else if (key == keyboard.getKey("MoveLeft")) {
				--target_c;
				dir = MoveDirection::LEFT;
			} else if (key == keyboard.getKey("MoveRight")) {
				++target_c;
				dir = MoveDirection::RIGHT;
			} else {
				return;
			}

			Result game_state = Result::ONGOING;

			if (current_mode == ControlMode::MOVE) {
				if (player.move(map, dir)) {
					Audio::getInstance().playSound("Move", true);
				}

				game_state = checkGameState(player, map);
			} else if (current_mode == ControlMode::MARK) {
				if (player.toggleMark(map.grid, target_r, target_c)) {
					Audio::getInstance().playSound("Mark", true);
				}
			}

			if (game_state != Result::ONGOING) {
				stopGame(
					game_state == Result::WIN ? "Win" : "Lose",
					player, map, ui
				);

				return;
			}

			ui.updateGamePage(map, player);
		}
	}

	void System::updateTimer(
		UI& ui
	) {
		sf::Time elapsed = clock.getElapsedTime();
		ui.updateTimerDisplay(static_cast<int>(elapsed.asSeconds()));
	}

	Result System::checkGameState(
		const Player& player,
		const Map& map
	) const {
		auto [r, c] = player.getPosition();
		
		if (map.grid[r][c].is_mine) {
			return Result::LOSE;
		} else if (map.getCellToGo() == 0) {
			return Result::WIN;
		} else {
			return Result::ONGOING;
		}
	}

	bool System::isPlaying() const {
		return playing;
	}

	void System::resetTimer() {
		clock.reset();
	}
}
