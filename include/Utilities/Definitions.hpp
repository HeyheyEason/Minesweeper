#pragma once

#include <ostream>
#include <format>

namespace Mines {
	enum class ExecutionState {
		DEFAULT = -1,
		SUCCESS = 0,
		FAILED = 1
	};

	inline std::ostream& operator<<(std::ostream& os, ExecutionState es) {
		os << static_cast<int>(es);
		return os;
	}

	inline thread_local ExecutionState execution_state = ExecutionState::SUCCESS;

	inline constexpr int MAX_SCORE = 10000;
	inline constexpr double TIME_WEIGHT = 10.0;
	inline constexpr double STEP_WEIGHT = 2.0;

	inline constexpr float DEFAULT_MUSIC_VOLUME = 100.f;
	inline constexpr float DEFAULT_SOUND_VOLUME = 100.f;
	inline constexpr int DEFAULT_FPS_LIMIT = 60;
	
#ifndef NDEBUG
	inline constexpr int DEBUG_MINES_COUNT = 5;
	inline constexpr int DEBUG_START_POSX = 0;
	inline constexpr int DEBUG_START_POSY = 0;
	inline constexpr float DEBUG_START_OFFSET = 30.0f;
	inline constexpr float DEBUG_LOOP_START = 0.0f;
	inline constexpr float DEBUG_LOOP_END = 60.0f;
#endif
} // namespace Mines

namespace std {
	template<>
	struct formatter<Mines::ExecutionState> : public formatter<int> {
		auto format(Mines::ExecutionState es, format_context& ctx) const {
			return formatter<int>::format(static_cast<int>(es), ctx);
		}
	};
} // namespace std
