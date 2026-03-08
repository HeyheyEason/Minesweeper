#include "Core/Map.hpp"
#include "Utilities/Randomizer.hpp"
#include "Utilities/Log.hpp"
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

    void Map::init(const std::set<int>& safe_cells, Difficulty df) {
        std::vector<int> block_indices(64);
        std::iota(block_indices.begin(), block_indices.end(), 0);

        std::random_device rd;
        std::mt19937 g(rd());
        Randomizer::getInstance().shuffle(block_indices);
        int placed = 0;

        for (int block_idx : block_indices) {
            if (placed >= static_cast<int>(df)) {
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

                if (safe_cells.count(idx) == 0) {
                    grid.at(r).at(c).is_mine = true;
                    ++placed;
                    break;
                }
            }
        }

        cell_to_go -= placed;
        countMines();
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

    void Map::countMines() {
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; ++j) {
                int sum = 0;

                for (int m = i - 1; m <= i + 1; ++m) {
                    for (int n = j - 1; n <= j + 1; ++n) {
                        if (m < 0 || m >= 16 || n < 0 || n >= 16) {
                            continue;
                        }

                        if (m == i && n == j) {
                            continue;
                        }

                        if (grid.at(m).at(n).is_mine) {
                            sum += 1;
                        }
                    }
                }

                grid.at(i).at(j).mines_around = sum;
            }
        }
    }
} // namespace Mines