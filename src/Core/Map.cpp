#include "Core/Map.hpp"
#include "Utilities/Randomizer.hpp"
#include "Utilities/Log.hpp"
#include "Utilities/Definitions.hpp"
#include <random>
#include <algorithm>
#include <numeric>

namespace Mines {
    std::ostream& operator<<(std::ostream& os, const Cell& c) {
        if (!c.is_revealed) {
            os << ". ";
        }
        else {
            if (c.is_mine) {
                os << "\033[91mX\033[0m ";
            }
            else if (c.player_here) {
                os << "\033[92mP\033[0m ";
            }
            else if (c.is_marked) {
                os << "\033[93mM\033[0m ";
            }
            else {
                os << c.mines_around << ' ';
            }
        }

        return os;
    }

    Map::Map() : grid(Grid(16, std::vector<Cell>(16))), cell_to_go(256) {}

    void Map::init(const std::set<int>& starting_safe_cells, Difficulty df) {
        int target_mines = static_cast<int>(df);
        int total_safe_cells = 256 - target_mines;

        int start_idx = *starting_safe_cells.begin();
        int start_r = start_idx / 16;
        int start_c = start_idx % 16;

        std::vector<int> block_indices(64);
        bool is_map_valid = false;
        int placed = 0;

        int dr[] = { -1, -1, -1,  0, 0,  1, 1, 1 };
        int dc[] = { -1,  0,  1, -1, 1, -1, 0, 1 };

        do {
            placed = 0;
            grid = Grid(16, std::vector<Cell>(16));
            cell_to_go = 256;

            std::iota(block_indices.begin(), block_indices.end(), 0);
            Randomizer::getInstance().shuffle(block_indices);

            for (int block_idx : block_indices) {
                if (placed >= target_mines) {
                    break;
                }

                int br = (block_idx / 8) * 2;
                int bc = (block_idx % 8) * 2;
                int pos[] = { 0, 1, 2, 3 };
                Randomizer::getInstance().shuffle(pos, pos + 4);

                for (int p : pos) {
                    int r = br + (p / 2);
                    int c = bc + (p % 2);
                    int idx = r * 16 + c;

                    if (starting_safe_cells.count(idx) == 0) {
                        grid.at(r).at(c).is_mine = true;
                        ++placed;

                        for (int i = 0; i < 8; ++i) {
                            int nr = r + dr[i];
                            int nc = c + dc[i];

                            if (nr >= 0 && nr < 16 && nc >= 0 && nc < 16) {
                                grid.at(nr).at(nc).mines_around += 1;
                            }
                        }

                        break;
                    }
                }
            }

            int accessible_safe_cells = 0;
            std::vector<std::vector<bool>> visited(16, std::vector<bool>(16, false));
            checkConnectivity(start_r, start_c, visited, accessible_safe_cells);

            if (accessible_safe_cells == total_safe_cells) {
                is_map_valid = true;
            }
        } while (!is_map_valid);
        
        cell_to_go  = total_safe_cells;
    }

    void Map::revealAllCells(bool value) {
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; ++j) {
                grid.at(i).at(j).is_revealed = value;
            }
        }
    }

    void Map::processCombo(int x, int y) {
        if (x < 0 || x >= 16 || y < 0 || y >= 16) {
            return;
        }

        if (grid.at(x).at(y).is_revealed) {
            return;
        }

        if (grid.at(x).at(y).is_mine) {
            return;
        }

        if (grid.at(x).at(y).mines_around == 0) {
            grid.at(x).at(y).is_revealed = true;
            --cell_to_go;

            int dx[] = { -1, 1, 0, 0 };
            int dy[] = { 0, 0, -1, 1 };

            for (int i = 0; i < 4; ++i) {
                processCombo(x + dx[i], y + dy[i]);
            }
        }
        else {
            return;
        }
    }

    void Map::reduceCellToGo() {
        --cell_to_go;
    }

    int Map::getCellToGo() const {
        return cell_to_go;
    }

    void Map::checkConnectivity(
        int r, int c,
        std::vector<std::vector<bool>>& visited,
        int& count
    ) const {
        if (r < 0 || r >= 16 || c < 0 || c >= 16) {
            return;
        }
            
        if (grid.at(r).at(c).is_mine || visited.at(r).at(c)) {
            return;
        }

        visited.at(r).at(c) = true;
        ++count;

        checkConnectivity(r + 1, c, visited, count);
        checkConnectivity(r - 1, c, visited, count);
        checkConnectivity(r, c + 1, visited, count);
        checkConnectivity(r, c - 1, visited, count);
    }
} // namespace Mines
