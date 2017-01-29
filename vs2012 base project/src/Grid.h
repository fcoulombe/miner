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
        for (int y = 0; y < kGridSize; ++y) {
            for (int x = 0; x < kGridSize; ++x) {
                (*this)[y][x].FadeOut();
            }
        }
    }
    void Reset() {
        for (int y = 0; y < kGridSize; ++y) {
            for (int x = 0; x < kGridSize; ++x) {
                int blockId = rand() % kNumTypeBlocks + 1;
                (*this)[y][x].SetTexture((King::Engine::Texture)(blockId));
                (*this)[y][x].FadeIn();
            }
        }
        mHadMatches = true;
    }

    void Render(King::Engine &engine) {
        Grid &grid = *(this);
        for (int y = 0; y < kGridSize; ++y) {
            for (int x = 0; x < kGridSize; ++x) {
                grid[y][x].Render(engine);
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
        return points;
    }

    int RemoveMatches();
    void CollapseGrid();

    std::vector<Block *> GetHorizontalMatchingBlocks();
    std::vector<Block *> GetVerticalMatchingBlocks();

    void SetHadMatches() { mHadMatches = true; }
private:
    bool mHadMatches = true;
};
