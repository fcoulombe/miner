#pragma once

#include <glm/glm.hpp>

constexpr float kBlockSize = 40.0f;
constexpr float kHalfBlockSize = kBlockSize / 2.0f;
constexpr float kGridOffsetX = 340.0f;
constexpr float kGridOffsetY = 120.0f;
constexpr int kGridSize = 8;
inline glm::ivec2 MapScreenToGrid(const glm::vec2 &screenPosition) {
    return glm::ivec2((screenPosition.x - kGridOffsetX) / kBlockSize, (screenPosition.y - kGridOffsetY) / kBlockSize);
}

inline glm::vec2 MapGridToScreen(const glm::vec2 &gridPosition) {
    return glm::vec2(gridPosition.x*kBlockSize + kGridOffsetX, gridPosition.y*kBlockSize + kGridOffsetY);
}

