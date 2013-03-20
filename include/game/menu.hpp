#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../../include/gui/GuiController.hpp"

#include "../../include/aw/utilities/converter.hpp"

#include "../../include/game/settings.hpp"

#include "../../include/game/MenuBackground.hpp"

#include "../../include/game/HighscoreUploader.hpp"

#include "../../include/game/HighscoreBoard.hpp"


class Menu
{
public:

    Menu(sf::RenderWindow &window);
    ~Menu();

    std::string Run();

    std::string GetLevelName();

private:

    void Init();

    void HandleEvents();

    void Draw();


    void InitGui();

    void StartGame(std::string lvlname);

    void InitBackground();


private:

    sf::RenderWindow &m_window;

    bool m_running;
    std::string m_returnValue;
    std::string m_levelName;

    GuiController m_gui;

    sf::Texture m_texHeadline;
    sf::Sprite  m_srHeadline;

    MenuBackground *m_pbg;

    sf::Music m_music;

    sf::RectangleShape m_underlay;
};


#endif // MENU_HPP
