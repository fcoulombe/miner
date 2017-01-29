#pragma once

#include <glm/glm.hpp>
#include "Grid.h"

class MinerDragEvent
{
public:
    MinerDragEvent() {}
    MinerDragEvent(const glm::vec2 &screenMousePos, Grid &grid);
    void OnDrag(const glm::vec2 &screenMousePos, Grid &grid);
    void StopDrag(const glm::vec2 &screenMousePos, Grid &grid);
    bool IsDragging(const glm::vec2 &screenMousePos) const;

    Block *GetSelection() const { return mDraggedBlock; }
private:
    void ResetPositions();
    glm::vec2 mStartDragMouseScreenPos;
    glm::ivec2 mStartDragGridPos;
    Block *mDraggedBlock=nullptr;
    
    glm::ivec2 mOppositeStartDragGridPos;
    Block *mOppositeDraggedBlock = nullptr;
};