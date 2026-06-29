#include "pch.h"
#include "Timer.h"

void Timer::SetCurrentTime()
{
    m_lastTime = std::chrono::steady_clock::now();
}

std::chrono::steady_clock::time_point Timer::GetCurrentTime()
{
	return m_currentTime = std::chrono::steady_clock::now();
}

double Timer::GetElapsed()
{
    auto now = std::chrono::steady_clock::now();
    m_elapsed = std::chrono::duration<double>(now - m_lastTime).count();
	m_lastTime = now;
	return m_elapsed;
}

void Timer::DebugTime()
{
    auto now = std::chrono::steady_clock::now();
    m_elapsed = std::chrono::duration<double>(now - m_lastTime).count();

    std::cerr << "Elapsed milliseconds: "
        << std::chrono::duration<double>(now - m_lastTime).count()
        << "ms\n";
}

void Timer::Sleep(float p_SecForSleep)
{
    auto start = std::chrono::steady_clock::now();

    while (true) 
    {
        auto now = std::chrono::steady_clock::now();
        double duration = std::chrono::duration<double>(now - start).count();
    
        if (duration >= p_SecForSleep)
        {
            break;
        }
    }
}

std::chrono::steady_clock::time_point Timer::GetLastTime()
{
    return m_lastTime;
}
