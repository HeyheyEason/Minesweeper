#include "Shell/Keyboard.hpp"
#include "Core/Table.hpp"
#include "Utilities/Log.hpp"
#include <ranges>

namespace Mines {
	Keyboard::Keyboard() {
		Table table;
		table.load("../../data/Config/KeyboardBinding.cfg");

		keys["MoveUp"] = static_cast<sf::Keyboard::Scancode>(table.get<int>("MoveUp", 22));
		keys["MoveDown"] = static_cast<sf::Keyboard::Scancode>(table.get<int>("MoveDown", 18));
		keys["MoveLeft"] = static_cast<sf::Keyboard::Scancode>(table.get<int>("MoveLeft", 0));
		keys["MoveRight"] = static_cast<sf::Keyboard::Scancode>(table.get<int>("MoveRight", 3));
		keys["SwitchMode"] = static_cast<sf::Keyboard::Scancode>(table.get<int>("SwitchMode", 40));
		keys["Pause"] = static_cast<sf::Keyboard::Scancode>(table.get<int>("Pause", 37));
	}

	void Keyboard::setCurrentAction(const std::string& action) {
		current_action = action;
		is_binding = true;
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
			LOG_WARN("Cannot bind the same key to {}.", current_action);
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

	void Keyboard::saveKeys() const {
		Table table;

		for (const auto& [action, key_code] : keys) {
			table.set(action, static_cast<int>(key_code));
		}

		table.save("../../data/Config/KeyboardBinding.cfg");
	}

	void Keyboard::reset() {
		keys["MoveUp"] = sf::Keyboard::Scancode::W;
		keys["MoveDown"] = sf::Keyboard::Scancode::S;
		keys["MoveLeft"] = sf::Keyboard::Scancode::A;
		keys["MoveRight"] = sf::Keyboard::Scancode::D;
		keys["SwitchMode"] = sf::Keyboard::Scancode::Space;
		keys["Pause"] = sf::Keyboard::Scancode::Escape;
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
			return "Num " + std::string(1, '1' + (static_cast<int>(key) - static_cast<int>(Numpad1)));
		} else if (key == Numpad0) {
			return "Num 0";
		}

		if (key >= F1 && key <= F12) {
			return "F" + std::to_string(static_cast<int>(key) - static_cast<int>(F1) + 1);
		}
		
		switch (key) {
			case Up:
				return "Up Arrow";
			case Down:
				return "Down Arrow";
			case Left:
				return "Left Arrow";
			case Right:
				return "Right Arrow";
			case Space:    
				return "Space";
			case Enter:    
				return "Enter";
			case NumpadEnter:
				return "Num Enter";
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
			case Unknown:
				return "Unknown";
			default:
				return "Key #" + std::to_string(static_cast<int>(key));
		}
	}
} // namespace Mines
