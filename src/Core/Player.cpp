#include "Core/Player.hpp"
#include "Shell/Audio.hpp"

namespace Mines {
    Player::Player(const std::string& name, int x, int y) :
        name(name), posx(x), posy(y), step(0), mark_count(0) {
    }

    bool Player::move(Map& map, const MoveDirection& dir) {
        int new_x = posx;
        int new_y = posy;

        switch (dir) {
        case MoveDirection::UP:
            --new_x;
            break;
        case MoveDirection::DOWN:
            ++new_x;
            break;
        case MoveDirection::LEFT:
            --new_y;
            break;
        case MoveDirection::RIGHT:
            ++new_y;
            break;
        default:
            return false;
        }

        if (new_x < 0 || new_x >= 16 || new_y < 0 || new_y >= 16) {
            return false;
        }

        map.grid.at(posx).at(posy).player_here = false;
        posx = new_x;
        posy = new_y;
        map.grid.at(posx).at(posy).player_here = true;
        ++step;

        if (!map.grid.at(posx).at(posy).is_revealed) {
            if (map.grid.at(posx).at(posy).mines_around > 0) {
                map.grid.at(posx).at(posy).is_revealed = true;
                map.reduceCellToGo();
            }
            else {
                Audio::getInstance().playSound("Safe");
                map.processCombo(posx, posy);
            }
        }

        return true;
    }

    bool Player::toggleMark(Grid& grid, int x, int y) {
        if (x < 0 || x >= 16 || y < 0 || y >= 16) {
            return false;
		}

        if (grid.at(x).at(y).is_marked) {
            grid.at(x).at(y).is_marked = false;
            --mark_count;
        }
        else {
            grid.at(x).at(y).is_marked = true;
            ++mark_count;
        }

        return true;
    }

    int Player::getStep() const {
        return step;
    }

    int Player::getMarkCount() const {
        return mark_count;
    }

    std::string Player::getName() const {
        return name;
    }

    std::pair<int, int> Player::getPosition() const {
        return { posx, posy };
    }
} // namespace Mines