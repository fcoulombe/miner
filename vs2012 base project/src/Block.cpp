#include <king/Engine.h>
#include "Block.h"
#include "TaskManager.h"

Block::Block()
    : mTexture((King::Engine::Texture)(rand() % 5 + 1))
    , mIsVisible(true)
{
    FadeIn();
}

void Block::Update(King::Engine &engine) {
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
        block->SetAlpha(1.0f*percentage);
    }, 1.0f);
}

void Block::MoveTo(int x, int y) {
    glm::vec2 destination(MapGridToScreen(glm::vec2((float)x, (float)y)));
    glm::vec2 origin = mPosition;
    glm::vec2 delta(destination - origin);
    Block *currentBlock = this;
    TaskManager::Instance().RunTaskOverTime([currentBlock, delta, origin](float percentage) {
        currentBlock->SetPosition(origin + (delta*percentage));
    }, 1.0f);
}