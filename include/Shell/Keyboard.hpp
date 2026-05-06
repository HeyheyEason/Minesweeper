#pragma once

#include "Core/Table.hpp"
#include <map>
#include <TGUI/Backend/SFML-Graphics.hpp>

namespace Mines {
	class Keyboard {
	public:
		Keyboard();

		void setCurrentAction(const std::string& action);
		void bind(sf::Keyboard::Key key);
		bool isBinding() const;

	private:
		std::map<std::string, sf::Keyboard::Key> keys;
		bool is_binding = false;
		std::string current_action;

		std::string toString(sf::Keyboard::Key key);
	};
} // namespace Mines
