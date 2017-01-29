#include <king/Engine.h>
#include "Block.h"
#include "TaskManager.h"

Block::Block()
    : mTexture((King::Engine::Texture)(rand() % kNumTypeBlocks + 1))
    , mIsVisible(true)
{
    FadeIn();
}

void Block::Render(King::Engine &engine) {
    if (!mIsVisible || MapScreenToGrid(GetPosition()).y < 0) {
        return;
    }
    // not all textures are the same size, so we position them in the center of a 40x40 block of space
    float x = mPosition.x + (kBlockSize - engine.GetTextureWidth(mTexture)) / 2.0f;
    float y = mPosition.y + (kBlockSize - engine.GetTextureHeight(mTexture)) / 2.0f;
    engine.Render(mTexture, x, y, 0.0f, mAlpha);
}

void Block::FadeIn() {
    Block *block = this;
    TaskManager::Instance().RunTaskOverTime([block](float percentage) {
        block->SetAlpha(kBlockDefaultAlpha*percentage);
    }, kFadeSpeed);
}


void Block::FadeOut() {
    Block *block = this;
    TaskManager::Instance().RunTaskOverTime([block](float percentage) {
        block->SetAlpha(kBlockDefaultAlpha*(1.0f-percentage));
    }, kFadeSpeed);
}

void Block::MoveTo(int x, int y, float time) {
    const glm::vec2 destination(MapGridToScreen(glm::vec2((float)x, (float)y)));
    const glm::vec2 origin = mPosition;
    const glm::vec2 delta(destination - origin);
    Block *currentBlock = this;
    TaskManager::Instance().RunTaskOverTime([currentBlock, delta, origin](float percentage) {
        currentBlock->SetPosition(origin + (delta*percentage));
    }, time);
}

