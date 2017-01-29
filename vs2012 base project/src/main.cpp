#define GLM_FORCE_RADIANS 

#include <king/Engine.h>
#include <king/Updater.h>

#include "Block.h"
#include "MinerDragEvent.h"
#include "TaskManager.h"
#include "Utils.h"

#include <string>
#include <ctime>

//**********************************************************************


class MinerGame : public King::Updater {
public:
    MinerGame()
        : mEngine("./assets") {
        srand((unsigned int)time(NULL));
    }

    void Start() {
        mEngine.Start(*this);
    }

    void Update() {
        float dt = mEngine.GetLastFrameSeconds();
        mTime -= dt*kGameSpeed;

        mEngine.Render(King::Engine::TEXTURE_BACKGROUND, 0.0f, 0.0f);
        mGrid.Render(mEngine);

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

        // run the animation. don't let the user do anything while it's animating
        if (TaskManager::Instance().IsBusy() || mIsFinished) {
            TaskManager::Instance().Update(mEngine.GetLastFrameSeconds());
            return;
        }

        mPoints += mGrid.Update();

        // perform inputs
        const auto screenMousePos = glm::vec2(mEngine.GetMouseX(), mEngine.GetMouseY());
        // start dragging
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
        // end dragging
        if (mEngine.GetMouseButtonUp() && mIsDragging) {
            if (mSelection)
                mSelection->Unselect();

            if (mDragEvent.IsDragging(screenMousePos)) {
                mDragEvent.StopDrag(screenMousePos, mGrid);
                mSelection = nullptr;
            }
            else {
                if (mSelection) {
                    Block *secondSelection = mDragEvent.GetSelection();
                    const glm::ivec2 selectionGridPos = MapScreenToGrid(mSelection->GetPosition());
                    const glm::ivec2 secondSelectionGridPos = MapScreenToGrid(secondSelection->GetPosition());
                    const glm::ivec2 delta = glm::abs(secondSelectionGridPos - selectionGridPos);
                    // if direct neighbor
                    if ((delta.x == 1 && delta.y == 0) || (delta.x == 0 && delta.y == 1)) {
                        std::swap(mGrid[selectionGridPos.y][selectionGridPos.x], mGrid[secondSelectionGridPos.y][secondSelectionGridPos.x]);
                        auto verticalMatchingBlocks = mGrid.GetVerticalMatchingBlocks();
                        auto horizontalMatchingBlocks = mGrid.GetHorizontalMatchingBlocks();
                        if (!verticalMatchingBlocks.empty() || !horizontalMatchingBlocks.empty()) {
                            mGrid.SetHadMatches();
                            mSelection->MoveTo(selectionGridPos.x, selectionGridPos.y, 0.1f);
                            secondSelection->MoveTo(secondSelectionGridPos.x, secondSelectionGridPos.y, 0.1f);
                        }
                        else {
                            std::swap(mGrid[selectionGridPos.y][selectionGridPos.x], mGrid[secondSelectionGridPos.y][secondSelectionGridPos.x]);
                        }

                    }
                    mSelection = nullptr;
                }
                else {
                    mSelection = mDragEvent.GetSelection();
                    mSelection->Select();
                }
            }
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

    Block *mSelection = nullptr;

};

//**********************************************************************

int main(int argc, char *argv[]) {
    MinerGame game;
    game.Start();

    return 0;
}


