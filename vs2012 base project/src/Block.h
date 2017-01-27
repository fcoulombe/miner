#pragma once

#include "Utils.h"

class Block {
public:
    Block();

    void Update(King::Engine &engine);

    void FadeIn();

    void MoveTo(int x, int y);

    void SetPosition(int x, int y)
    {
        SetPosition(MapGridToScreen(glm::ivec2(x, y)));
    }
    void SetPosition(const glm::ivec2 &position)
    {
        SetPosition(MapGridToScreen(position));
    }
    void SetPosition(const glm::vec2 &position)
    {
        mPosition = position;
    }
    const glm::vec2 &GetPosition() const { return mPosition; }

    void SetTexture(King::Engine::Texture texture) { mTexture = texture; }
    King::Engine::Texture GetTexture() const { return mTexture; }
    void SetVisible(bool isVisible) { mIsVisible = isVisible; }
    bool IsVisible() const { return mIsVisible; }

    void SetAlpha(float alpha) { mAlpha = alpha; }
private:
    King::Engine::Texture mTexture;
    glm::vec2 mPosition;
    bool mIsVisible;
    float mAlpha = 0.0f;
};
