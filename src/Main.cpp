#include "../include/game/Menu.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/Settings.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Image.hpp>

#include <string>
#include <cstdlib> // srand
#include <ctime> // time

int main()
{
    std::srand(std::time(nullptr));


    sf::RenderWindow window;

    window.create(sf::VideoMode(800,450), "Kroniax", sf::Style::Default, sf::ContextSettings(24,8,4));
    window.setFramerateLimit(200);
    window.setKeyRepeatEnabled(false);

    sf::Image tex;
    tex.loadFromFile("data/sprites/icon.png");
    window.setIcon(64,64,tex.getPixelsPtr());

    settings::Load();

    while(window.isOpen())
    {
        Menu menu(window);

        std::string value = menu.Run();

        if(value == "exit")
        {
            window.close();
        }

        if(value == "game")
        {
            std::string returnValue;

            do
            {
                Game game(window);

                settings::SetSpeedY(0);
                settings::SetDownforce(settings::GetStartDownforce());
                settings::SetSpeedX(settings::GetStartSpeedX());

                returnValue = game.Run(menu.GetLevelName());

            }while(returnValue == "restart");

        }
    }

    settings::Save();

    return 0;
}
