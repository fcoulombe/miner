#pragma once

#include <functional>
#include <list>

class InterpolationTask {
public:
    InterpolationTask(std::function<void(float)> task, float duration);
    bool Update(float dt);
private:
    std::function<void(float)> mTask;
    float mDuration;
    float mTime = 0.0f;
};

class TaskManager {
public:
    static TaskManager &Instance();
    void RunTaskOverTime(std::function<void(float)> task, float duration);
    void RunTask(std::function<void()> task);
    void Update(float dt);

    bool IsBusy() const { return !mInterpolationTaskList.empty(); }
private:
    TaskManager() {}
    std::list<InterpolationTask> mInterpolationTaskList;
    std::list<std::function<void()>> mTaskList;
};
