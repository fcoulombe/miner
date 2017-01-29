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

bool MinerDragEvent::IsDragging(const glm::vec2 &screenMousePos) const {
    const glm::vec2 delta = screenMousePos - mStartDragMouseScreenPos;
    const glm::vec2 absDelta = glm::abs(delta);
    if (absDelta.x < kDragMovementThreshold && absDelta.y < kDragMovementThreshold) {
        return false;
    }
    return true;
}

void MinerDragEvent::OnDrag(const glm::vec2 &screenMousePos, Grid &grid) {
    const glm::vec2 delta = screenMousePos - mStartDragMouseScreenPos;
    const glm::vec2 absDelta = glm::abs(delta);
    ResetPositions();
    
    // we only want to move the block if the drag is more than 15 pixels
    if (absDelta.x < kDragMovementThreshold && absDelta.y < kDragMovementThreshold) {
        return;
    }

    const glm::vec2 startBlockScreenPos = MapGridToScreen(mStartDragGridPos);
    // if drag vertical
    if (absDelta.y > absDelta.x) {
        if (delta.y > 0.0f) {
            if (mStartDragGridPos.y + 1 < kGridSize) {
                float dragLimit = startBlockScreenPos.y + kBlockSize + kHalfBlockSize;
                const auto newPos = glm::vec2(startBlockScreenPos.x,
                                              glm::min(screenMousePos.y, dragLimit) - kHalfBlockSize);
                mDraggedBlock->SetPosition(newPos);

                mOppositeStartDragGridPos = glm::ivec2(mStartDragGridPos.x, mStartDragGridPos.y + 1);
                mOppositeDraggedBlock = &(grid[mOppositeStartDragGridPos.y][mOppositeStartDragGridPos.x]);
                const glm::vec2 startOppositeScreenPos = MapGridToScreen(mOppositeStartDragGridPos);
                mOppositeDraggedBlock->SetPosition(glm::vec2(startOppositeScreenPos.x,
                                                             startOppositeScreenPos.y - (newPos.y - startBlockScreenPos.y)));
            }
        }
        else {
            if (mStartDragGridPos.y - 1 >= 0) {
                float dragLimit = startBlockScreenPos.y - kBlockSize + kHalfBlockSize;
                const auto newPos = glm::vec2(startBlockScreenPos.x,
                                              glm::max(screenMousePos.y, dragLimit) - kHalfBlockSize);
                mDraggedBlock->SetPosition(newPos);

                mOppositeStartDragGridPos = glm::ivec2(mStartDragGridPos.x, mStartDragGridPos.y - 1);
                mOppositeDraggedBlock = &(grid[mOppositeStartDragGridPos.y][mOppositeStartDragGridPos.x]);
                const glm::vec2 startOppositeScreenPos = MapGridToScreen(mOppositeStartDragGridPos);
                mOppositeDraggedBlock->SetPosition(glm::vec2(startOppositeScreenPos.x,
                                                            startOppositeScreenPos.y - (newPos.y - startBlockScreenPos.y)));
            }
        }
    }
    else {
        if (delta.x > 0.0f) {
            if (mStartDragGridPos.x + 1 < kGridSize) {
                float dragLimit = startBlockScreenPos.x + kBlockSize + kHalfBlockSize;
                auto newPos = glm::vec2(glm::min(screenMousePos.x, dragLimit) - kHalfBlockSize,
                                        startBlockScreenPos.y);
                mDraggedBlock->SetPosition(newPos);

                mOppositeStartDragGridPos = glm::ivec2(mStartDragGridPos.x + 1, mStartDragGridPos.y);
                mOppositeDraggedBlock = &(grid[mOppositeStartDragGridPos.y][mOppositeStartDragGridPos.x]);
                const glm::vec2 startOppositeScreenPos = MapGridToScreen(mOppositeStartDragGridPos);
                mOppositeDraggedBlock->SetPosition(glm::vec2(startOppositeScreenPos.x - (newPos.x - startBlockScreenPos.x), 
                                                            startOppositeScreenPos.y));
            }
        }
        else {
            if (mStartDragGridPos.x - 1 >= 0) {
                float dragLimit = startBlockScreenPos.x - kBlockSize + kHalfBlockSize;
                auto newPos = glm::vec2(glm::max(screenMousePos.x, dragLimit) - kHalfBlockSize,
                                        startBlockScreenPos.y);
                mDraggedBlock->SetPosition(newPos);
                
                mOppositeStartDragGridPos = glm::ivec2(mStartDragGridPos.x - 1, mStartDragGridPos.y);
                mOppositeDraggedBlock = &(grid[mOppositeStartDragGridPos.y][mOppositeStartDragGridPos.x]);
                const glm::vec2 startOppositeScreenPos = MapGridToScreen(mOppositeStartDragGridPos);
                mOppositeDraggedBlock->SetPosition(glm::vec2(startOppositeScreenPos.x - (newPos.x - startBlockScreenPos.x), 
                                                             startOppositeScreenPos.y));
            }
        }
    }
}

void MinerDragEvent::StopDrag(const glm::vec2 &screenMousePos, Grid &grid) {
    bool hasSwapped = false;
    if (mOppositeDraggedBlock) {
        auto oppositeGridPos = MapScreenToGrid(mOppositeDraggedBlock->GetPosition());
        if (oppositeGridPos == mStartDragGridPos) {
            Block &lhs = grid[mOppositeStartDragGridPos.y][mOppositeStartDragGridPos.x];
            Block &rhs = grid[mStartDragGridPos.y][mStartDragGridPos.x];
            lhs.SetPosition(glm::ivec2(mStartDragGridPos.x, mStartDragGridPos.y));
            rhs.SetPosition(glm::ivec2(mOppositeStartDragGridPos.x, mOppositeStartDragGridPos.y));
            std::swap(lhs, rhs);
            
            auto verticalMatchingBlocks = grid.GetVerticalMatchingBlocks();
            auto horizontalMatchingBlocks = grid.GetHorizontalMatchingBlocks();
            if (!verticalMatchingBlocks.empty() || !horizontalMatchingBlocks.empty()) {
                hasSwapped = true;
                grid.SetHadMatches();
            }
            else {
                std::swap(lhs, rhs);
                hasSwapped = false;
            }
        }

        if (!hasSwapped) {
            mOppositeDraggedBlock->MoveTo(mOppositeStartDragGridPos.x, mOppositeStartDragGridPos.y, 0.1f);
        }
    }
    if (!hasSwapped) {
        mDraggedBlock->MoveTo(mStartDragGridPos.x, mStartDragGridPos.y, 0.1f);
    }
}