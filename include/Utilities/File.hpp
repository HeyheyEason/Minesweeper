#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <map>
#include <sstream>

namespace Mines {
	class File {
	public:
		virtual ~File() = default;

		void load(const std::filesystem::path& path);
		void save(const std::filesystem::path& path);



	protected:
		virtual void onBeforeLoad() = 0;
		virtual void parseLine(const std::string& line) = 0;
		virtual std::vector<std::string> convertToLines() = 0;
	};
} // namespace Mines
