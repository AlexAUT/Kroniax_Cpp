#ifndef GAME_HPP
#define GAME_HPP

#include "level.hpp"

#include "highscoreUploader.hpp"
#include "HighscoreBoard.hpp"

#include "../../include/aw/utilities/stopwatch.hpp"

class Game
{
public:
    Game(sf::RenderWindow &window);

    ~Game();

    std::string Run(std::string levelName);

private:

    void Init();


    void HandleEvents();

    void DoLogic();

    void Draw();

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

#endif
