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
private:
    void ResetPositions();
    glm::vec2 mStartDragMouseScreenPos;
    glm::ivec2 mStartDragGridPos;
    Block *mDraggedBlock=nullptr;
    
    glm::ivec2 mOppositeStartDragGridPos;
    Block *mOppositeDraggedBlock = nullptr;
};