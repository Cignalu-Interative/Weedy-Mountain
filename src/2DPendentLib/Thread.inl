#pragma once
#include "Thread.h"

template<typename Func, typename... Args>
Thread::Thread(Func&& func, Args&&... args)
    : m_thread(std::forward<Func>(func), std::forward<Args>(args)...)
{
}