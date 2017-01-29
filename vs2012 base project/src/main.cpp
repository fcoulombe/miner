#define GLM_FORCE_RADIANS 

#include <king/Engine.h>
#include <king/Updater.h>

#include "Block.h"
#include "MinerDragEvent.h"
#include "TaskManager.h"
#include "Utils.h"

//**********************************************************************


class MinerGame : public King::Updater {
public:


    MinerGame()
        : mEngine("./assets")
        , mIsDragging(false) {
    }

    void Start() {
        mEngine.Start(*this);
    }

    void Update() {
        mEngine.Render(King::Engine::TEXTURE_BACKGROUND, 0.0f, 0.0f);

        mGrid.Render(mEngine);

        //run the animation. don't let the user do anything while it's animating
        if (TaskManager::Instance().IsBusy()) {
            TaskManager::Instance().Update(mEngine.GetLastFrameSeconds());
            return;
        }

        mGrid.Update();

        // perform inputs
        const auto screenMousePos = glm::vec2(mEngine.GetMouseX(), mEngine.GetMouseY());
        //start dragging
        if (mEngine.GetMouseButtonDown() && IsWithinGrid(screenMousePos) && !mIsDragging) {
            mIsDragging = true;
            mDragEvent = MinerDragEvent(screenMousePos, mGrid);
        }
        // perform dragging
        if (mIsDragging) {
            mDragEvent.OnDrag(screenMousePos, mGrid);
        }
        //end dragging
        if (mEngine.GetMouseButtonUp() && mIsDragging) {
            mDragEvent.StopDrag(screenMousePos, mGrid);
            mIsDragging = false;
        }
    }

private:
    King::Engine mEngine;
    Grid mGrid;
    bool mIsDragging;
    
    MinerDragEvent mDragEvent;

};

//**********************************************************************

int main(int argc, char *argv[]) {
    MinerGame game;
    game.Start();

    return 0;
}


