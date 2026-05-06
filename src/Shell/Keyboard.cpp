#include "Shell/Keyboard.hpp"
#include "Utilities/Log.hpp"
#include <ranges>

namespace Mines {
	Keyboard::Keyboard() {
		keys["MoveUp"] = sf::Keyboard::Key::W;
		keys["MoveDown"] = sf::Keyboard::Key::S;
		keys["MoveLeft"] = sf::Keyboard::Key::A;
		keys["MoveRight"] = sf::Keyboard::Key::D;
		keys["SwitchMode"] = sf::Keyboard::Key::Space;
		keys["Pause"] = sf::Keyboard::Key::Escape;
	}

	void Keyboard::setCurrentAction(const std::string& action) {
		current_action = action;
	}

	void Keyboard::bind(sf::Keyboard::Key key) {
		bool exists = std::ranges::any_of(keys, [key](const auto& pair) {
			return pair.second == key;
		});

		if (!exists) {
			keys[current_action] = key;
			is_binding = false;
		} else {
			LOG_ERROR("Cannot bind the same key to {}.", current_action);
		}
	}

	bool Keyboard::isBinding() const {
		return is_binding;
	}

	std::string Mines::Keyboard::toString(sf::Keyboard::Key key) {
		auto scan_code = sf::Keyboard::delocalize(key);
		sf::String name = sf::Keyboard::getDescription(scan_code);

		if (name.isEmpty()) {
			return "Key #" + std::to_string(static_cast<int>(key));
		}

		return name;
	}
} // namespace Mines
