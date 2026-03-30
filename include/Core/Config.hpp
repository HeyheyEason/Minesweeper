#pragma once

#include "Utilities/File.hpp"
#include <map>
#include <string>

namespace Mines {
	class Config : public File {
	public:
		void set(const std::string& key, const auto& val) {
			std::stringstream ss;
			ss << val;
			data[key] = val;
		}

		auto get(const std::string& key, auto default_value) {
			auto it = data.find(key);

			if (it == data.end()) {
				return default_value;
			}

			if constexpr (std::is_convertible_v<decltype(default_value), std::string>) {
				return it->second;
			} else {
				std::stringstream ss(it->second);
				decltype(default_value) result{};
				return (ss >> result) ? result : default_value;
			}
		}

	protected:
		void onBeforeLoad() override;
		void parseLine(const std::string& line) override;
		std::vector<std::string> convertToLines() override;

	private:
		std::map<std::string, std::string> data;
	};
} // namespace Mines
