#include "Core/Config.hpp"

namespace Mines {
	void Config::onBeforeLoad() {
		data.clear();
	}

	void Config::parseLine(const std::string& line) {
		size_t sep = line.find('=');

		if (sep != std::string::npos) {
			std::string k = line.substr(0, sep);
			std::string v = line.substr(sep + 1);

			auto trim = [](std::string& str) {
				str.erase(0, str.find_first_not_of(" \t"));
				str.erase(0, str.find_last_not_of(" \t") + 1);
			};

			trim(k);
			trim(v);

			if (!k.empty()) {
				data[k] = v;
			}
		}
	}

	std::vector<std::string> Config::convertToLines() {
		std::vector<std::string> out;

		for (const auto& [k, v] : data) {
			out.push_back(k + "=" + v);
		}

		return out;
	}
} // namespace Mines
