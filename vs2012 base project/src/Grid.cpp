#include "Grid.h"

int Grid::RemoveMatches() {
    Grid &grid = *(this);
    auto verticalMatchingBlocks = GetVerticalMatchingBlocks();
    for (auto block : verticalMatchingBlocks) {
        block->SetVisible(false);
    }
    auto horizontalMatchingBlocks = GetHorizontalMatchingBlocks();
    for (auto block : horizontalMatchingBlocks) {
        block->SetVisible(false);
    }
    return verticalMatchingBlocks.size() + horizontalMatchingBlocks.size();
}

void Grid::CollapseGrid() {
    Grid &grid = *(this);
    for (int x = 0; x < kGridSize; ++x) {
        int aboveGridHeight = -1; //stack falling blocks
        for (int y = kGridSize - 1; y >= 0; --y) {
            Block &currentBlock = grid[y][x];
            if (currentBlock.IsVisible())
                continue;

            bool hasValidBlockAbove = false;
            // look at blocks above to make them fall
            for (int i = y - 1; i >= 0; --i) {
                Block &blockAbove = grid[i][x];
                if (!blockAbove.IsVisible())
                    continue;
                std::swap(currentBlock, blockAbove);
                currentBlock.MoveTo(x, y);
                hasValidBlockAbove = true;
                break;
            }
            // if there's no block above, create a new one.
            if (!hasValidBlockAbove) {
                int blockId = rand() % kNumTypeBlocks + 1;
                currentBlock.SetTexture((King::Engine::Texture)(blockId));
                currentBlock.SetVisible(true);
                currentBlock.SetPosition(x, aboveGridHeight);
                --aboveGridHeight;
                currentBlock.MoveTo(x, y);
            }
        }
    }
}

std::vector<Block *> Grid::GetHorizontalMatchingBlocks() {
    Grid &grid = *this;
    std::vector<Block *> horizontalMatchingBlocks;
    for (int y = 0; y < kGridSize; ++y) {
        std::vector<int> consecutiveBlocks;
        for (int x = 0; x < kGridSize; ++x) {
            if (!consecutiveBlocks.empty() &&
                grid[y][consecutiveBlocks.back()].GetTexture() == grid[y][x].GetTexture()) {
                consecutiveBlocks.emplace_back(x);
            }
            else {
                if (consecutiveBlocks.size() >= 3) {
                    //destroy blocks
                    for (int k = 0; k < (int)consecutiveBlocks.size(); ++k) {
                        horizontalMatchingBlocks.emplace_back(&grid[y][consecutiveBlocks[k]]);
                    }
                }
                consecutiveBlocks.clear();
                consecutiveBlocks.emplace_back(x);
            }
        }
        if (consecutiveBlocks.size() >= 3) {
            for (int k = 0; k < (int)consecutiveBlocks.size(); ++k) {
                horizontalMatchingBlocks.emplace_back(&grid[y][consecutiveBlocks[k]]);
            }
        }
    }
    return horizontalMatchingBlocks;
}

std::vector<Block *> Grid::GetVerticalMatchingBlocks() {

    Grid &grid = *this;
    std::vector<Block*> verticalMatchingBlocks;
    for (int x = 0; x < kGridSize; ++x) {
        std::vector<int> consecutiveBlocks;
        for (int y = 0; y < kGridSize; ++y) {
            if (!consecutiveBlocks.empty() &&
                grid[consecutiveBlocks.back()][x].GetTexture() == grid[y][x].GetTexture()) {
                consecutiveBlocks.emplace_back(y);
            }
            else {
                if (consecutiveBlocks.size() >= 3) {
                    for (int k = 0; k < (int)consecutiveBlocks.size(); ++k) {
                        verticalMatchingBlocks.emplace_back(&grid[consecutiveBlocks[k]][x]);
                    }
                }
                consecutiveBlocks.clear();
                consecutiveBlocks.emplace_back(y);
            }
        }
        if (consecutiveBlocks.size() >= 3) {
            for (int k = 0; k < (int)consecutiveBlocks.size(); ++k) {
                verticalMatchingBlocks.emplace_back(&grid[consecutiveBlocks[k]][x]);
            }
        }
    }
    return verticalMatchingBlocks;
}


