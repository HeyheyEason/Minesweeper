#include "Shell/Keyboard.hpp"
#include "Utilities/Log.hpp"
#include <ranges>

namespace Mines {
	Keyboard::Keyboard() {
		keys["MoveUp"] = sf::Keyboard::Scancode::W;
		keys["MoveDown"] = sf::Keyboard::Scancode::S;
		keys["MoveLeft"] = sf::Keyboard::Scancode::A;
		keys["MoveRight"] = sf::Keyboard::Scancode::D;
		keys["SwitchMode"] = sf::Keyboard::Scancode::Space;
		keys["Pause"] = sf::Keyboard::Scancode::Escape;
	}

	void Keyboard::setCurrentAction(const std::string& action) {
		current_action = action;
	}

	std::pair<std::string, std::string> Keyboard::bind(sf::Keyboard::Scancode key) {
		bool exists = std::ranges::any_of(keys, [key](const auto& pair) {
			return pair.second == key;
		});

		if (!exists) {
			keys[current_action] = key;
			is_binding = false;
			return { current_action, toString(key) };
		} else {
			LOG_ERROR("Cannot bind the same key to {}.", current_action);
			return { "", "" };
		}
	}

	bool Keyboard::isBinding() const {
		return is_binding;
	}

	sf::Keyboard::Scancode Keyboard::getKey(const std::string& action) const {
		auto it = keys.find(action);

		if (it != keys.end()) {
			return it->second;
		} else {
			return sf::Keyboard::Scancode::Unknown;
		}
	}

	std::string Mines::Keyboard::toString(sf::Keyboard::Scancode key) {
		using enum sf::Keyboard::Scancode;
		
		if (key >= A && key <= Z) {
			return std::string(1, 'A' + (static_cast<int>(key) - static_cast<int>(A)));
		}
		
		if (key >= Num1 && key <= Num9) {
			return std::string(1, '1' + (static_cast<int>(key) - static_cast<int>(Num1)));
		} else if (key == Num0) {
			return "0";
		}

		if (key >= Numpad1 && key <= Numpad9) {
			return "Num " + std::string(1, (static_cast<int>(key) - static_cast<int>(Num1)));
		} else if (key == Numpad0) {
			return "Num 0";
		}

		if (key >= F1 && key <= F12) {
			return "F" + std::to_string(static_cast<int>(key) - static_cast<int>(F1) + 1);
		}
		
		switch (key) {
			case Space:    
				return "Space";
			case Enter:    
				return "Enter";
			case Escape:   
				return "Esc";
			case LControl: 
				return "Left Ctrl";
			case RControl: 
				return "Right Ctrl";
			case LShift:   
				return "Left Shift";
			case RShift:   
				return "Right Shift";
			case LAlt:     
				return "Left Alt";
			case RAlt:     
				return "Right Alt";
			case Backspace:
				return "Backspace";
			case Tab:      
				return "Tab";
			default:
				return "Key #" + std::to_string(static_cast<int>(key));
		}
	}
} // namespace Mines
