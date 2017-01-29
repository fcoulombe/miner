#include "Block.h"

#include <array>
#include <vector>

class Grid : public std::array<std::array<Block, kGridSize>, kGridSize> {
public:
    Grid() {
        for (int y = 0; y < kGridSize; ++y) {
            for (int x = 0; x < kGridSize; ++x) {
                (*this)[y][x].SetPosition(x, y);
            }
        }
    }

    void Render(King::Engine &engine) {
        Grid &grid = *(this);
        for (int y = 0; y < kGridSize; ++y) {
            for (int x = 0; x < kGridSize; ++x) {
                grid[y][x].Render(engine);
            }
        }

    }
    void Update() {
        if (mHadMatches) {
            mHadMatches = RemoveMatches();
            CollapseGrid();
        }
    }

    bool RemoveMatches();
    void CollapseGrid();

    std::vector<Block *> GetHorizontalMatchingBlocks();
    std::vector<Block *> GetVerticalMatchingBlocks();

    void SetHadMatches() { mHadMatches = true; }
private:
    bool mHadMatches = true;
};