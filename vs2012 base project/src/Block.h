#pragma once

#include <king/Engine.h>
#include "Utils.h"

class Block {
public:
    Block();

    void Render(King::Engine &engine);

    void FadeIn();
    void FadeOut();

    void Select() {
        SetAlpha(kBlockSelectedAlpha);
    }

    void Unselect() {
        SetAlpha(kBlockDefaultAlpha);
    }

    void MoveTo(int x, int y, float time = 0.3f);

    void SetPosition(int x, int y) {
        SetPosition(MapGridToScreen(glm::ivec2(x, y)));
    }
    void SetPosition(const glm::ivec2 &position) {
        SetPosition(MapGridToScreen(position));
    }
    void SetPosition(const glm::vec2 &position) {
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
    bool mIsVisible = true;
    float mAlpha = 0.0f;
};


