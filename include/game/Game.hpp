#ifndef GAME_HPP
#define GAME_HPP

#include "../../include/aw/utilities/StopWatch.hpp"

class Level;
class Tracer;

#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>

namespace sf
{
	class RenderWindow;
}

#include <string>


class Game
{
public:
    Game(sf::RenderWindow &window);

    ~Game();

    std::string Run(std::string levelName, Tracer &currentTracer, Tracer &lastTracer);

private:

    void Init();


    void HandleEvents();

    void DoLogic(Tracer &currentTracer);

    void Draw(Tracer &currentTracer, Tracer &lastTracer);

    void PauseTheGame();
    void Finish();

    void CreateBody();

    void SetWindowName();

private:

    sf::RenderWindow &m_window;

    std::string m_returnValue;

    std::string m_levelName;

    Stopwatch m_gameTime;
    sf::Clock m_frameTime;
    double m_lastFrameTime;

    bool m_running;

    sf::Vector2f m_position;
    sf::Vertex m_body[3]; // Triangle

    Level *m_pLevel;

    sf::Vector2f m_viewPositionFloat;
    sf::View m_view;

    sf::Font m_font;
    sf::Text m_time;
    sf::Text m_speedX;

    sf::Music m_music;

};

#endif // GAME_HPP
