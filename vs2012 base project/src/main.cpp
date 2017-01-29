#define GLM_FORCE_RADIANS 

#include <king/Engine.h>
#include <king/Updater.h>

#include "Block.h"
#include "MinerDragEvent.h"
#include "TaskManager.h"
#include "Utils.h"

#include <string>

//**********************************************************************


class MinerGame : public King::Updater {
public:


    MinerGame()
        : mEngine("./assets") {
    }

    void Start() {
        mEngine.Start(*this);
    }

    void Update() {
        float dt = mEngine.GetLastFrameSeconds();

        mEngine.Render(King::Engine::TEXTURE_BACKGROUND, 0.0f, 0.0f);
        mGrid.Render(mEngine);

        constexpr float kSpeed = 1.0f;
        mTime -= dt*kSpeed;
        if (!mIsFinished) {
            mEngine.Write(std::to_string(std::max(int(mTime), 0)).c_str(), 100.0f, 450.0f, 0.0f);
            if (mTime <= 0.0f) {
                mIsFinished = true;
                mTime = kGameFinishedDuration;
                mGrid.FadeOut();
            }
        } else {
            mEngine.Write("Finished", 450.0f, 250.0f, 0.0f);
            if (mTime <= 0.0f) {
                mIsFinished = false;
                mTime = kGameDuration;
                mPoints = 0;
                mGrid.Reset();
            }
        }
        
        mEngine.Write(std::to_string(mPoints).c_str(), 100.0f, 150.0f, 0.0f);


        //run the animation. don't let the user do anything while it's animating
        if (TaskManager::Instance().IsBusy()) {
            TaskManager::Instance().Update(mEngine.GetLastFrameSeconds());
            return;
        }

        mPoints += mGrid.Update();

        // perform inputs
        const auto screenMousePos = glm::vec2(mEngine.GetMouseX(), mEngine.GetMouseY());
        //start dragging
        if (mEngine.GetMouseButtonDown() && IsWithinGrid(screenMousePos)) {
            if (!mIsDragging) {
                mIsDragging = true;
                mDragEvent = MinerDragEvent(screenMousePos, mGrid);
            }
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
    bool mIsDragging = false;
    MinerDragEvent mDragEvent;
    float mTime = kGameDuration;
    bool mIsFinished = false;
    int mPoints = 0;

};

//**********************************************************************

int main(int argc, char *argv[]) {
    MinerGame game;
    game.Start();

    return 0;
}


