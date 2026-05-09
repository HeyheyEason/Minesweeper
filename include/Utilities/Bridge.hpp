#pragma once

#include <string>
#include <functional>

namespace Mines {
	struct Bridge {
		std::function<void(const std::string&, int, int, int)> start_game;
		std::function<void()> resume_game;
		std::function<void(const std::string&)> stop_game;
		std::function<void(const std::string&)> bind_action;
		std::function<bool()> is_binding;
	};
} // namespace Mines
