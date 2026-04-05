#pragma once

#include "Utilities/File.hpp"
#include <string>
#include <vector>

namespace Mines {
	class List : public File {
	public:
		bool set(size_t index, const auto& val) {
			if (index >= data.size()) {
				return false;
			}

			std::stringstream ss;
			ss << val;
			data[index] = ss.str();
			return true;
		}

		std::string get(size_t index, const char* default_value) {
			return get<std::string>(index, std::string(default_value));
		}

		auto get(size_t index, auto default_value) {
			if (index >= data.size()) {
				return default_value;
			}

			if constexpr (std::is_convertible_v<decltype(default_value), std::string>) {
				return data[index];
			} else {
				std::stringstream ss(data[index]);
				decltype(default_value) result{};
				return (ss >> result) ? result : default_value;
			}
		}

		void pushBack(const auto& val) {
			std::stringstream ss;
			ss << val;
			data.push_back(ss.str());
		};

		void popBack();
		size_t getSize() const;

	protected:
		void onBeforeLoad() override;
		void parseLine(const std::string& line) override;
		std::vector<std::string> convertToLines() override;

	private:
		std::vector<std::string> data;
	};
} // namespace Mines
