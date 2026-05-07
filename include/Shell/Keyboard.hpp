#pragma once

#include "Core/Table.hpp"
#include <utility>
#include <map>
#include <TGUI/Backend/SFML-Graphics.hpp>

namespace Mines {
	class Keyboard {
	public:
		Keyboard();

		void setCurrentAction(const std::string& action);
		std::pair<std::string, std::string> bind(sf::Keyboard::Scancode key);
		bool isBinding() const;
		sf::Keyboard::Scancode getKey(const std::string& action) const;

	private:
		std::map<std::string, sf::Keyboard::Scancode> keys;
		bool is_binding = false;
		std::string current_action;

		std::string toString(sf::Keyboard::Scancode key);
	};
} // namespace Mines
