#pragma once

#include <string>
#include <functional>

namespace Mines {
	struct Adapter {
		std::function<void(const std::string&, int, int, int)> start_game;
		std::function<void()> resume_game;
		std::function<void(const std::string&)> stop_game;
	};
} // namespace Mines