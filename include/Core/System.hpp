#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <string>
#include "Core/Map.hpp"
#include "Core/Player.hpp"
#include "Shell/UI.hpp"

namespace Mines {
	enum class Result {
		WIN,
		LOSE,
		ONGOING
	};

	enum class ControlMode {
		MOVE,
		MARK
	};

	class System {
	public:
		System() = default;

		void startGame(
			const std::string& name,
			int x,
			int y,
			int df_index,
			Player& player,
			Map& map,
			UI& ui
		);
		
		void resumeGame();

		void stopGame(
			const std::string& result,
			const Player& player,
			Map& map,
			UI& ui
		);

		void handleInput(
			const sf::Event& event,
			Player& player,
			Map& map,
			UI& ui
		);

		void updateTimer(
			UI& ui
		);

		Result checkGameState(
			const Player& player,
			const Map& map
		) const;

		bool isPlaying() const;
		void resetTimer();

	private:
		ControlMode current_mode = ControlMode::MOVE;
		sf::Clock clock;
		int difficulty = 1;
		bool playing = false;
	};
} // namespace Mines