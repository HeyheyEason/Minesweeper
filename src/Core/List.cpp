#include "Core/List.hpp"

namespace Mines {
	void List::popBack() {
		data.pop_back();
	}

	size_t List::getSize() const {
		return data.size();
	}

	void List::onBeforeLoad() {
		data.clear();
	}

	void List::parseLine(const std::string& line) {
		data.push_back(line);
	}

	std::vector<std::string> List::convertToLines() {
		return data;
	}
} // namespace Mines
