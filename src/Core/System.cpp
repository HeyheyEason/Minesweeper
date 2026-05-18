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

		map.init(safe_cells, static_cast<Difficulty>(5 * difficulty + 30));
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

	void System::handleMessages(Player& player, Map& map, UI& ui) {
		for (auto msg = ui.pollMessage(); msg.type != MessageType::NULL_MSG; msg = ui.pollMessage()) {
			const MessageType current_type = msg.type;
			bool should_return = false;

			std::visit(
				[this, current_type, &should_return, &player, &map, &ui](auto&& arg) {
					using T = std::decay_t<decltype(arg)>;

					if constexpr (std::is_same_v<T, Data::StartGame>) {
						startGame(arg.player_name, arg.x, arg.y, arg.df_index, player, map, ui);
					} else if constexpr (std::is_same_v<T, Data::StopGame>) {
						stopGame(arg.result, player, map, ui);
					} else if constexpr (std::is_same_v<T, Data::Null>) {
						switch (current_type) {
							case MessageType::PAUSE_GAME:
								playing = false;
								clock.stop();
								should_return = true;
								return;
							case MessageType::RESUME_GAME:
								resumeGame();
								should_return = true;
								return;
							case MessageType::PLAYER_SWITCH_MODE:
								current_mode = (current_mode == ControlMode::MOVE) ? ControlMode::MARK : ControlMode::MOVE;
								ui.setModeIndicator(static_cast<int>(current_mode));
								should_return = true;
								return;
						}

						auto [r, c] = player.getPosition();
						int target_r = r;
						int target_c = c;
						MoveDirection dir;

						switch (current_type) {
							case MessageType::PLAYER_PRESS_UP:
								--target_r;
								dir = MoveDirection::UP;
								break;
							case MessageType::PLAYER_PRESS_DOWN:
								++target_r;
								dir = MoveDirection::DOWN;
								break;
							case MessageType::PLAYER_PRESS_LEFT:
								--target_c;
								dir = MoveDirection::LEFT;
								break;
							case MessageType::PLAYER_PRESS_RIGHT:
								++target_c;
								dir = MoveDirection::RIGHT;
								break;
							default:
								should_return = true;
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
						}
					}
				},
				msg.data
			);

			if (should_return) {
				return;
			}
		}
	}

	void System::updateTimer(UI& ui) {
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
