#ifndef MENU_HPP
#define MENU_HPP

#include "../../include/gui/GuiController.hpp"

class MenuBackground;

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Audio/Music.hpp>

namespace sf
{
	class RenderWindow;
}

#include <string>

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
