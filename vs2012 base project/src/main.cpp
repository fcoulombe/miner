#define GLM_FORCE_RADIANS 

#include <vector>

#include <king/Engine.h>
#include <king/Updater.h>

#include "Block.h"
#include "TaskManager.h"
#include "Utils.h"

//**********************************************************************


class MinerGame : public King::Updater {
public:

    std::vector<Block *> GetHorizontalMatchingBlocks()
    {
        std::vector<Block *> horizontalMatchingBlocks;
        for (int y = 0; y < kGridSize; ++y)
        {
            std::vector<int> consecutiveBlocks;
            for (int x = 0; x < kGridSize; ++x)
            {
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
    std::vector<Block *> GetVerticalMatchingBlocks() {
        std::vector<Block*> verticalMatchingBlocks;
        for (int x = 0; x < kGridSize; ++x)
        {
            std::vector<int> consecutiveBlocks;
            for (int y = 0; y < kGridSize; ++y)
            {
                if (!consecutiveBlocks.empty() &&
                    grid[consecutiveBlocks.back()][x].GetTexture() == grid[y][x].GetTexture()) {
                    consecutiveBlocks.emplace_back(y);
                }
                else {
                    if (consecutiveBlocks.size() >= 3) {
                        for (int k = 0; k < (int)consecutiveBlocks.size(); ++k)
                        {
                            verticalMatchingBlocks.emplace_back(&grid[consecutiveBlocks[k]][x]);
                        }
                    }
                    consecutiveBlocks.clear();
                    consecutiveBlocks.emplace_back(y);
                }
            }
            if (consecutiveBlocks.size() >= 3) {
                for (int k = 0; k < (int)consecutiveBlocks.size(); ++k)
                {
                    verticalMatchingBlocks.emplace_back(&grid[consecutiveBlocks[k]][x]);
                }
            }
        }
        return verticalMatchingBlocks;
    }
    bool RemoveMatches()
    {
        auto verticalMatchingBlocks = GetVerticalMatchingBlocks();
        for (auto block : verticalMatchingBlocks) {
            block->SetVisible(false);
        }
        auto horizontalMatchingBlocks = GetHorizontalMatchingBlocks();
        for (auto block : horizontalMatchingBlocks) {
            block->SetVisible(false);
        }
        return !verticalMatchingBlocks.empty() || !horizontalMatchingBlocks.empty();
    }

    void CollapseGrid()
    {
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
                    int blockId = rand() % 5 + 1;
                    currentBlock.SetTexture((King::Engine::Texture)(blockId));
                    currentBlock.SetVisible(true);
                    currentBlock.SetPosition(x, aboveGridHeight);
                    --aboveGridHeight;
                    currentBlock.MoveTo(x, y);
                }
            }
        }
    }
    MinerGame()
        : mEngine("./assets")
        , mIsDragging(false) {
        for (int y = 0; y < kGridSize; ++y)
        {
            for (int x = 0; x < kGridSize; ++x)
            {
                grid[y][x].SetPosition(x, y);
            }
        }
    }

    void Start() {
        mEngine.Start(*this);
    }

    bool IsWithinGrid(const glm::vec2 &screenMousePos) const
    {
        return gridMousePos.x >= 0 && gridMousePos.x < kGridSize &&
            gridMousePos.y >= 0 && gridMousePos.y < kGridSize;
    }
    void OnMouseDown(const glm::vec2 &screenMousePos) {
        const auto gridMousePos = MapScreenToGrid(screenMousePos);
        if (IsWithinGrid(screenMousePos)) {
            if (!mIsDragging) {
                mIsDragging = true;
                mStartDragScreenPos = screenMousePos;
                mStartDragGridPos = gridMousePos;
                mDraggedBlock = &(grid[gridMousePos.y][gridMousePos.x]);
            }
        }
    }

    void UpdateOppositeDragBlock(const glm::ivec2 &oppositeGridPos) {
        Block *tempOppositeBlock = &(grid[oppositeGridPos.y][oppositeGridPos.x]);
        if (mOppositeDraggedBlock == nullptr) {
            mOppositeStartDragGridPos = oppositeGridPos;
        }
        // different opposite block. reset old one
        else if (tempOppositeBlock != mOppositeDraggedBlock) {
            mOppositeDraggedBlock->SetPosition(mOppositeStartDragGridPos);
            mOppositeStartDragGridPos = oppositeGridPos;
        }
        mOppositeDraggedBlock = tempOppositeBlock;
    }

    void OnMouseDrag(const glm::vec2 &screenMousePos) {
        float deltaX = screenMousePos.x - mStartDragScreenPos.x;
        float absDeltaX = abs(deltaX);
        float deltaY = screenMousePos.y - mStartDragScreenPos.y;
        float absDeltaY = abs(deltaY);

        constexpr float kMovementThreshold = 15.0f;
        // we only want to move the block if the drag is more than 15 pixels
        if (absDeltaX < kMovementThreshold && absDeltaY < kMovementThreshold) {
            mDraggedBlock->SetPosition(MapGridToScreen(mStartDragGridPos));
            if (mOppositeDraggedBlock) {
                mOppositeDraggedBlock->SetPosition(mOppositeStartDragGridPos);
            }
            return;
        }

        const glm::vec2 originalBlockScreenPos = MapGridToScreen(mStartDragGridPos);
        // if drag vertical
        if (absDeltaY > absDeltaX) {
            if (deltaY > 0.0f) {
                if (mStartDragGridPos.y + 1 < kGridSize) {
                    float dragLimit = originalBlockScreenPos.y + kBlockSize + kHalfBlockSize;
                    auto newPos = glm::vec2(originalBlockScreenPos.x,
                                                 glm::min(screenMousePos.y, dragLimit) - kHalfBlockSize);
                    mDraggedBlock->SetPosition(newPos);

                    const auto oppositeGridPos = glm::ivec2(mStartDragGridPos.x, mStartDragGridPos.y + 1);
                    UpdateOppositeDragBlock(oppositeGridPos);
                    const glm::vec2 originalOppositeScreenPos = MapGridToScreen(oppositeGridPos);
                    mOppositeDraggedBlock->SetPosition(glm::vec2(originalOppositeScreenPos.x, originalOppositeScreenPos.y - (newPos.y - MapGridToScreen(mStartDragGridPos).y)));
                }
            }
            // drag down and there's a block down.
            else {
                if (mStartDragGridPos.y - 1 >= 0) {
                    float dragLimit = originalBlockScreenPos.y - kBlockSize + kHalfBlockSize ;
                    auto newPos = glm::vec2(originalBlockScreenPos.x,
                                            glm::max(screenMousePos.y, dragLimit) - kHalfBlockSize);
                    mDraggedBlock->SetPosition(newPos);

                    const auto oppositeGridPos = glm::ivec2(mStartDragGridPos.x, mStartDragGridPos.y - 1);
                    UpdateOppositeDragBlock(oppositeGridPos);
                    const glm::vec2 originalOppositeScreenPos = MapGridToScreen(oppositeGridPos);
                    mOppositeDraggedBlock->SetPosition(glm::vec2(originalOppositeScreenPos.x, originalOppositeScreenPos.y - (newPos.y - originalBlockScreenPos.y)));
                }
            }
        }
        else {
            if (deltaX > 0.0f) {
                if (mStartDragGridPos.x + 1 < kGridSize) {
                    float dragLimit = originalBlockScreenPos.x + kBlockSize + kHalfBlockSize;
                    auto newPos = glm::vec2(glm::min(screenMousePos.x, dragLimit) - kHalfBlockSize, originalBlockScreenPos.y);
                    mDraggedBlock->SetPosition(newPos);

                    const auto oppositeGridPos = glm::ivec2(mStartDragGridPos.x+1, mStartDragGridPos.y);
                    UpdateOppositeDragBlock(oppositeGridPos);
                    const glm::vec2 originalOppositeScreenPos = MapGridToScreen(oppositeGridPos);
                    mOppositeDraggedBlock->SetPosition(glm::vec2(originalOppositeScreenPos.x - (newPos.x - originalBlockScreenPos.x), originalOppositeScreenPos.y));

                }
            }
            else {
                if (mStartDragGridPos.x - 1 >= 0) {
                    float dragLimit = originalBlockScreenPos.x - kBlockSize + kHalfBlockSize;
                    auto newPos = glm::vec2(glm::max(screenMousePos.x,dragLimit) - kHalfBlockSize, originalBlockScreenPos.y);
                    mDraggedBlock->SetPosition(newPos);

                    const auto oppositeGridPos = glm::ivec2(mStartDragGridPos.x- 1, mStartDragGridPos.y);
                    UpdateOppositeDragBlock(oppositeGridPos);
                    const glm::vec2 originalOppositeScreenPos = MapGridToScreen(oppositeGridPos);
                    mOppositeDraggedBlock->SetPosition(glm::vec2(originalOppositeScreenPos.x - (newPos.x - originalBlockScreenPos.x), originalOppositeScreenPos.y));

                }
            }
        }
    }

    void OnMouseUp(const glm::vec2 &screenMousePos) {
        if (mIsDragging) {
            bool hasSwapped = false;
            if (mOppositeDraggedBlock) {
                auto oppositeGridPos = MapScreenToGrid(mOppositeDraggedBlock->GetPosition());
                if (oppositeGridPos == mStartDragGridPos) {
                    Block tempBlock = grid[mOppositeStartDragGridPos.y][mOppositeStartDragGridPos.x];
                    grid[mOppositeStartDragGridPos.y][mOppositeStartDragGridPos.x] = grid[mStartDragGridPos.y][mStartDragGridPos.x];
                    grid[mStartDragGridPos.y][mStartDragGridPos.x] = tempBlock;
                    auto verticalMatchingBlocks = GetVerticalMatchingBlocks();
                    auto horizontalMatchingBlocks = GetHorizontalMatchingBlocks();
                    if (verticalMatchingBlocks.size() || horizontalMatchingBlocks.size()) {
                        mHadMatches = RemoveMatches();
                        CollapseGrid();
                        hasSwapped = true;
                    }
                    else {
                        Block tempBlock = grid[mOppositeStartDragGridPos.y][mOppositeStartDragGridPos.x];
                        grid[mOppositeStartDragGridPos.y][mOppositeStartDragGridPos.x] = grid[mStartDragGridPos.y][mStartDragGridPos.x];
                        grid[mStartDragGridPos.y][mStartDragGridPos.x] = tempBlock;
                        hasSwapped = false;
                    }
                }
                
                if (!hasSwapped) {
                    mOppositeDraggedBlock->SetPosition(mOppositeStartDragGridPos);
                }
            }
            if (!hasSwapped) {
                mDraggedBlock->SetPosition(MapGridToScreen(mStartDragGridPos));
            }
            mDraggedBlock = nullptr;
            mIsDragging = false;

        }

    }

    void Update() {
        mEngine.Render(King::Engine::TEXTURE_BACKGROUND, 0.0f, 0.0f);

        for (int y = 0; y < kGridSize; ++y)
        {
            for (int x = 0; x < kGridSize; ++x)
            {
                grid[y][x].Update(mEngine);
            }
        }

        //run the animation. don't let the user do anything while it's animating
        if (TaskManager::Instance().IsBusy()) {
            TaskManager::Instance().Update(mEngine.GetLastFrameSeconds());
            return;
        }

        if (mHadMatches) {
            mHadMatches = RemoveMatches();
            CollapseGrid();
        }
        
        const auto screenMousePos = glm::vec2(mEngine.GetMouseX(), mEngine.GetMouseY());
        if (mEngine.GetMouseButtonDown()) {
            OnMouseDown(screenMousePos);
        }
        if (mIsDragging) {
            OnMouseDrag(screenMousePos);
        }
        if (mEngine.GetMouseButtonUp()) {
            OnMouseUp(screenMousePos);
        }
    }

private:
    Block grid[kGridSize][kGridSize];
    King::Engine mEngine;
    bool mIsDragging;
    glm::vec2 mStartDragScreenPos;
    glm::ivec2 mStartDragGridPos;
    Block *mDraggedBlock = nullptr;

    //when dragging a block, its opposite needs to move as well to swap position
    glm::ivec2 mOppositeStartDragGridPos;
    Block *mOppositeDraggedBlock = nullptr;
    bool mHadMatches = true;
};

//**********************************************************************

int main(int argc, char *argv[]) {
    MinerGame game;
    game.Start();

    return 0;
}


