#include "Utilities/File.hpp"
#include <thread>

namespace Mines {
	void File::load(const std::filesystem::path& path) {
		std::ifstream ifs(path);

		if (!ifs) {
			return;
		}

		this->onBeforeLoad();
		std::string line;

		while (getline(ifs, line)) {
			if (line.empty() || line[0] == '#' || line[0] == ';') {
				continue;
			}

			this->parseLine(line);
		}
	}

	void File::save(const std::filesystem::path& path) {
		std::vector<std::string> lines = this->convertToLines();

		std::thread(
			[path, lines]() {
				std::ofstream ofs(path);

				for (const auto& line : lines) {
					ofs << line << '\n';
				}
			}
		).detach();
	}
} // namespace Mines
