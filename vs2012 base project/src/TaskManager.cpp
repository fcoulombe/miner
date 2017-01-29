#include "TaskManager.h"

InterpolationTask::InterpolationTask(std::function<void(float)> task, float duration) {
    mTask = task;
    mDuration = duration;
}

bool InterpolationTask::Update(float dt) {
    mTime += dt;
    if (mTime >= mDuration) {
        mTask(1.0f);
        return false;
    }
    else {
        mTask(mTime / mDuration);
        return true;
    }
}

TaskManager &TaskManager::Instance() {
    static TaskManager instance;
    return instance;
}
void TaskManager::RunTaskOverTime(std::function<void(float)> task, float duration) {
    mInterpolationTaskList.emplace_back(InterpolationTask(task, duration));
}

void TaskManager::Update(float dt) {
    for (auto taskIt = mInterpolationTaskList.begin(); taskIt != mInterpolationTaskList.end(); ) {
        if (!taskIt->Update(dt)) {
            //remove
            taskIt = mInterpolationTaskList.erase(taskIt);
        }
        else {
            ++taskIt;
        }
    }
}