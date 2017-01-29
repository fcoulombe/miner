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

    void FadeOut() {
        Grid &grid = *(this);
        for (auto &&row : grid) {
            for (auto &&block : row) {
                block.FadeOut();
            }
        }
    }
    void Reset() {
        Grid &grid = *(this);
        for (auto &&row : grid) {
            for (auto &&block : row) {
                int blockId = rand() % kNumTypeBlocks + 1;
                block.SetTexture((King::Engine::Texture)(blockId));
                block.FadeIn();
            }
        }
        mHadMatches = true;
    }

    void Render(King::Engine &engine) {
        Grid &grid = *(this);
        for (auto &&row : grid) {
            for (auto &&block : row) {
                block.Render(engine);
            }
        }

    }
    int Update() {
        int points = 0;
        if (mHadMatches) {
            points = RemoveMatches();
            mHadMatches = points != 0;
            CollapseGrid();
        }
        return points*kPointMultiplier;
    }

    int RemoveMatches();
    void CollapseGrid();

    std::vector<Block *> GetHorizontalMatchingBlocks();
    std::vector<Block *> GetVerticalMatchingBlocks();

    void SetHadMatches() { mHadMatches = true; }
private:
    bool mHadMatches = true;
};
