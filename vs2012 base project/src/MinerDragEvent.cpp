#include "MinerDragEvent.h"
#include "Utils.h"



MinerDragEvent::MinerDragEvent(const glm::vec2 &screenMousePos, Grid &grid) {
    mStartDragMouseScreenPos = screenMousePos;
    mStartDragGridPos = MapScreenToGrid(screenMousePos);
    mDraggedBlock = &(grid[mStartDragGridPos.y][mStartDragGridPos.x]);
    mOppositeDraggedBlock = nullptr;
}

void MinerDragEvent::ResetPositions() {
    if (mOppositeDraggedBlock) {
        mOppositeDraggedBlock->SetPosition(mOppositeStartDragGridPos);
        mOppositeDraggedBlock = nullptr;
    }
    mDraggedBlock->SetPosition(mStartDragGridPos);
}
void MinerDragEvent::OnDrag(const glm::vec2 &screenMousePos, Grid &grid) {
    const glm::vec2 delta = screenMousePos - mStartDragMouseScreenPos;
    const glm::vec2 absDelta = glm::abs(delta);
    ResetPositions();
    
    constexpr float kMovementThreshold = 15.0f;
    // we only want to move the block if the drag is more than 15 pixels
    if (absDelta.x < kMovementThreshold && absDelta.y < kMovementThreshold) {
        return;
    }

    const glm::vec2 originalBlockScreenPos = MapGridToScreen(mStartDragGridPos);
    // if drag vertical
    if (absDelta.y > absDelta.x) {
        if (delta.y > 0.0f) {
            if (mStartDragGridPos.y + 1 < kGridSize) {
                float dragLimit = originalBlockScreenPos.y + kBlockSize + kHalfBlockSize;
                auto newPos = glm::vec2(originalBlockScreenPos.x,
                    glm::min(screenMousePos.y, dragLimit) - kHalfBlockSize);
                mDraggedBlock->SetPosition(newPos);

                mOppositeStartDragGridPos = glm::ivec2(mStartDragGridPos.x, mStartDragGridPos.y + 1);
                mOppositeDraggedBlock = &(grid[mOppositeStartDragGridPos.y][mOppositeStartDragGridPos.x]);
                const glm::vec2 originalOppositeScreenPos = MapGridToScreen(mOppositeStartDragGridPos);
                mOppositeDraggedBlock->SetPosition(glm::vec2(originalOppositeScreenPos.x, originalOppositeScreenPos.y - (newPos.y - originalBlockScreenPos.y)));
            }
        }
        else {
            if (mStartDragGridPos.y - 1 >= 0) {
                float dragLimit = originalBlockScreenPos.y - kBlockSize + kHalfBlockSize;
                auto newPos = glm::vec2(originalBlockScreenPos.x,
                                        glm::max(screenMousePos.y, dragLimit) - kHalfBlockSize);
                mDraggedBlock->SetPosition(newPos);

                mOppositeStartDragGridPos = glm::ivec2(mStartDragGridPos.x, mStartDragGridPos.y - 1);
                mOppositeDraggedBlock = &(grid[mOppositeStartDragGridPos.y][mOppositeStartDragGridPos.x]);
                const glm::vec2 originalOppositeScreenPos = MapGridToScreen(mOppositeStartDragGridPos);
                mOppositeDraggedBlock->SetPosition(glm::vec2(originalOppositeScreenPos.x, originalOppositeScreenPos.y - (newPos.y - originalBlockScreenPos.y)));
            }
        }
    }
    else {
        if (delta.x > 0.0f) {
            if (mStartDragGridPos.x + 1 < kGridSize) {
                float dragLimit = originalBlockScreenPos.x + kBlockSize + kHalfBlockSize;
                auto newPos = glm::vec2(glm::min(screenMousePos.x, dragLimit) - kHalfBlockSize, originalBlockScreenPos.y);
                mDraggedBlock->SetPosition(newPos);

                mOppositeStartDragGridPos = glm::ivec2(mStartDragGridPos.x + 1, mStartDragGridPos.y);
                mOppositeDraggedBlock = &(grid[mOppositeStartDragGridPos.y][mOppositeStartDragGridPos.x]);
                const glm::vec2 originalOppositeScreenPos = MapGridToScreen(mOppositeStartDragGridPos);
                mOppositeDraggedBlock->SetPosition(glm::vec2(originalOppositeScreenPos.x - (newPos.x - originalBlockScreenPos.x), 
                                                            originalOppositeScreenPos.y));
            }
        }
        else {
            if (mStartDragGridPos.x - 1 >= 0) {
                float dragLimit = originalBlockScreenPos.x - kBlockSize + kHalfBlockSize;
                auto newPos = glm::vec2(glm::max(screenMousePos.x, dragLimit) - kHalfBlockSize, originalBlockScreenPos.y);
                mDraggedBlock->SetPosition(newPos);
                
                mOppositeStartDragGridPos = glm::ivec2(mStartDragGridPos.x - 1, mStartDragGridPos.y);
                mOppositeDraggedBlock = &(grid[mOppositeStartDragGridPos.y][mOppositeStartDragGridPos.x]);
                const glm::vec2 originalOppositeScreenPos = MapGridToScreen(mOppositeStartDragGridPos);
                mOppositeDraggedBlock->SetPosition(glm::vec2(originalOppositeScreenPos.x - (newPos.x - originalBlockScreenPos.x), 
                                                    originalOppositeScreenPos.y));
            }
        }
    }
}

void MinerDragEvent::StopDrag(const glm::vec2 &screenMousePos, Grid &grid) {
    bool hasSwapped = false;
    if (mOppositeDraggedBlock) {
        auto oppositeGridPos = MapScreenToGrid(mOppositeDraggedBlock->GetPosition());
        if (oppositeGridPos == mStartDragGridPos) {
            std::swap(grid[mOppositeStartDragGridPos.y][mOppositeStartDragGridPos.x], 
                      grid[mStartDragGridPos.y][mStartDragGridPos.x]);

            auto verticalMatchingBlocks = grid.GetVerticalMatchingBlocks();
            auto horizontalMatchingBlocks = grid.GetHorizontalMatchingBlocks();
            if (!verticalMatchingBlocks.empty() || !horizontalMatchingBlocks.empty()) {
                hasSwapped = true;
                grid.SetHadMatches();
            }
            else {
                std::swap(grid[mOppositeStartDragGridPos.y][mOppositeStartDragGridPos.x], 
                            grid[mStartDragGridPos.y][mStartDragGridPos.x]);
                hasSwapped = false;
            }
        }

        if (!hasSwapped) {
            mOppositeDraggedBlock->MoveTo(mOppositeStartDragGridPos.x, mOppositeStartDragGridPos.y, 0.3f);
        }
    }
    if (!hasSwapped) {
        mDraggedBlock->MoveTo(mStartDragGridPos.x, mStartDragGridPos.y, 0.3f);
    }
}