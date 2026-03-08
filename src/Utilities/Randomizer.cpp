#include "Utilities/Randomizer.hpp"
#include <algorithm>

namespace Mines {
	Randomizer& Randomizer::getInstance() {
		static Randomizer instance;
		return instance;
	}

	int Randomizer::getInt(int min, int max) {
		std::lock_guard<std::mutex> lock(mtx);
		std::uniform_int_distribution<int> dist(min, max);
		return dist(rng);
	}

	float Randomizer::getFloat(float lower_bound, float upper_bound) {
		std::lock_guard<std::mutex> lock(mtx);
		std::uniform_real_distribution<float> dist(lower_bound, upper_bound);
		return dist(rng);
	}

	void Randomizer::shuffle(std::vector<int>& vec) {
		std::lock_guard<std::mutex> lock(mtx);
		std::shuffle(vec.begin(), vec.end(), rng);
	}

	void Randomizer::shuffle(int* begin, int* end) {
		std::lock_guard<std::mutex> lock(mtx);
		std::shuffle(begin, end, rng);
	}

	Randomizer::Randomizer() {
		std::random_device rd;
		rng.seed(rd());
	}
} // namespace Mines