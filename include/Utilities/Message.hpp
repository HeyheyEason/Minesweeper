#pragma once

#include <string>
#include <variant>

namespace Mines {
	namespace Data {
		struct StartGame {
			std::string player_name;
			int x;
			int y;
			int df_index;
		};

		struct StopGame {
			std::string result;
		};

		struct Null {};
	} // namespace Data

	using MessageData = std::variant<Data::Null, Data::StartGame, Data::StopGame>;

	enum class MessageType {
		NULL_MSG = -1,
		START_GAME,
		PAUSE_GAME,
		RESUME_GAME,
		STOP_GAME,
		PLAYER_SWITCH_MODE,
		PLAYER_PRESS_UP,
		PLAYER_PRESS_DOWN,
		PLAYER_PRESS_LEFT,
		PLAYER_PRESS_RIGHT
	};

	struct Message {
		MessageType type;
		MessageData data;
	};
} // namespace Mines
