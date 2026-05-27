#pragma once
#include "ThreadManager.h"

template<typename T>
void ThreadManager::AddTaskWithValue(const std::function<void(T)> function, T value)
{
    std::lock_guard<std::mutex> lock(m_threadMutex);
    m_threads.push_back(std::make_unique<Thread>(
        [function, value]() {
            function(value);
        }
    ));
}