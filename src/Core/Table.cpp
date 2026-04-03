#include "Core/Table.hpp"

namespace Mines {
	void Table::onBeforeLoad() {
		data.clear();
	}

	void Table::parseLine(const std::string& line) {
		size_t sep = line.find('=');

		if (sep != std::string::npos) {
			std::string k = line.substr(0, sep);
			std::string v = line.substr(sep + 1);

			auto trim = [](std::string& str) {
				const std::string whitespaces = " \t\r\n";

				size_t first = str.find_first_not_of(whitespaces);

				if (first == std::string::npos) {
					str.clear();
					return;
				}

				size_t last = str.find_last_not_of(whitespaces);
				str = str.substr(first, (last - first + 1));
			};

			trim(k);
			trim(v);

			if (!k.empty()) {
				data[k] = v;
			}
		}
	}

	std::vector<std::string> Table::convertToLines() {
		std::vector<std::string> out;

		for (const auto& [k, v] : data) {
			out.push_back(k + "=" + v);
		}

		return out;
	}
} // namespace Mines
