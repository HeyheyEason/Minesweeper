#pragma once

#include <random>
#include <vector>
#include <mutex>

namespace Mines {
	class Randomizer {
	public:
		static Randomizer& getInstance();
		Randomizer(const Randomizer&) = delete;
		void operator=(const Randomizer&) = delete;
		int getInt(int min, int max);
		float getFloat(float lower_bound, float upper_bound);
		void shuffle(std::vector<int>& vec);
		void shuffle(int* begin, int* end);

	private:
		std::mt19937 rng;
		std::mutex mtx;
		Randomizer();
	};
} // namespace Mines