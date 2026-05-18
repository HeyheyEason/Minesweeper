#pragma once

#include <ostream>
#include <vector>
#include <set>

namespace Mines {
    enum class Difficulty {
        EASY = 30,
        MEDIUM = 35,
        HARD = 40
    };

    struct Cell {
        bool is_mine = false;
        bool is_revealed = false;
        bool is_marked = false;
        bool player_here = false;
        int mines_around = 0;
    };

    std::ostream& operator<<(std::ostream& os, const Cell& c);
    using Grid = std::vector<std::vector<Cell>>;

    class Map {
    public:
        Grid grid;
        Map();
        void init(const std::set<int>& safe_cells, Difficulty df);
        void revealAllCells(bool value);
        void processCombo(int x, int y);
        void reduceCellToGo();
        int getCellToGo() const;

    private:
        void checkConnectivity(
            int r, int c,
            std::vector<std::vector<bool>>& visited,
            int& count
        ) const;

        int cell_to_go;
    };
} // namespace Mines
