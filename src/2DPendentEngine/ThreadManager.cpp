#include "pch.h"
#include "ThreadManager.h"

ThreadManager::ThreadManager()
{
	m_threads.reserve(std::thread::hardware_concurrency() - 1); // nombre de cœurs logiques de la machine -1 pour ne pas bloquer le thread principal
    m_logs.reserve(256); // dépend de la verbosité de tes logs
}

ThreadManager::~ThreadManager()
{
    JoinAll();
}

ThreadManager* ThreadManager::Get()
{
    static ThreadManager instance;
    return &instance;
}

void ThreadManager::RunAsync(const std::function<void()>& task, const std::function<void()>& onDone)
{
    std::lock_guard<std::mutex> lock(m_threadMutex);
    m_threads.push_back(std::make_unique<Thread>(
        [task, onDone]() {
            task();
            onDone();
        }
    ));
}

void ThreadManager::AddTask(const std::function<void()> function)
{
    std::lock_guard<std::mutex> lock(m_threadMutex);
    m_threads.push_back(std::make_unique<Thread>(function));
}

void ThreadManager::JoinAll()
{
    std::lock_guard<std::mutex> lock(m_threadMutex);
    for (auto& thread : m_threads)
        thread->Join();
    m_threads.clear();
}

void ThreadManager::UpdateValue(int delta)
{
    std::lock_guard<std::mutex> lock(m_dataMutex);
    m_value += delta;
}

void ThreadManager::LogWork(int value)
{
    std::lock_guard<std::mutex> lock(m_dataMutex);
    m_logs.push_back(value);
}

int ThreadManager::GetValue() const
{
    std::lock_guard<std::mutex> lock(m_dataMutex);
    return m_value;
}