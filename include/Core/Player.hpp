#pragma once

#include "Core/Map.hpp"
#include <string>
#include <utility>

namespace Mines {
    enum class MoveDirection : char {
        UP = 'w',
        DOWN = 's',
        LEFT = 'a',
        RIGHT = 'd'
    };

    class Player {
    public:
        Player(const std::string& name, int x, int y);
        bool move(Map& map, const MoveDirection& dir);
        bool toggleMark(Grid& grid, int x, int y);
        int getStep() const;
        int getMarkCount() const;
        std::string getName() const;
        std::pair<int, int> getPosition() const;

    private:
        std::string name;
        int posx;
        int posy;
        int step;
        int mark_count;
    };
} // namespace Mines