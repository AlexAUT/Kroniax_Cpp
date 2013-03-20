#include "../../../include/aw/utilities/stopwatch.hpp"

Stopwatch::Stopwatch() : m_state(stopped), m_lastTime(0)
{

}

void Stopwatch::restart()
{
    m_lastTime = 0;
    m_time.restart();
    m_state = running;
}

void Stopwatch::pause()
{
    if(m_state == running)
    {
        m_state = paused;
        m_lastTime += m_time.getElapsedTime().asMilliseconds();
    }
}

void Stopwatch::resume()
{
    if(m_state == paused)
    {
        m_state = running;
        m_time.restart();
    }

}

void Stopwatch::stop()
{
    m_state = stopped;
}

unsigned int Stopwatch::GetTimeInMs()
{
    if(m_state == stopped)
    {
        return 0;
    }
    else if(m_state == running)
    {
        m_lastTime += m_time.getElapsedTime().asMilliseconds();
        m_time.restart();

        return m_lastTime;
    }
    else if(m_state == paused)
    {
        return m_lastTime;
    }

    return 0;
}

