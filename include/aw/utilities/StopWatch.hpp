#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

#include <SFML/System/Clock.hpp>

class Stopwatch
{
public:

    enum State
    {
        running,
        paused,
        stopped
    };

    Stopwatch();

    void restart();
    void pause();
    void resume();
    void stop();

    unsigned int GetTimeInMs();

private:

    State m_state;

    float m_lastTime;

    sf::Clock m_time;

};

#endif // STOPWATCH_HPP
