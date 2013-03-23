#include "../include/game/Menu.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/Tracer.hpp"
#include "../include/game/Settings.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Image.hpp>

#include <memory>
#include <string>
#include <cstdlib> // srand
#include <ctime> // time

#include <iostream>

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
            //game.Run will return a signal, restart, (more will follow)
            std::string returnValue;

            // Create 2pointer to keep track of the last and the current try of a level
            // After the level the addresses will be swaped and the current tracer will be reseted
            std::unique_ptr<Tracer> currentTracer(new Tracer);
            std::unique_ptr<Tracer> lastTracer(new Tracer);
            // Set the time between two points. Lower times can increase the performance
            currentTracer->SetTimeBetweenPoints(settings::GetTimeForTracer());
            lastTracer->SetTimeBetweenPoints(settings::GetTimeForTracer());

            do
            {
                Game game(window);

                //Define the start movement speed of the ship and the gravitation
                settings::SetSpeedY(0);
                settings::SetDownforce(settings::GetStartDownforce());
                settings::SetSpeedX(settings::GetStartSpeedX());
                //Run the game
                returnValue = game.Run(menu.GetLevelName(), *currentTracer, *lastTracer);

                //Swap the addressed and clear the two tries old version to store the new one in the next try
                //Only swap if the new Tracer is longer then the old one
                if(currentTracer->GetSize() >= lastTracer->GetSize())
                {
                    std::swap(currentTracer, lastTracer);
                }
                //Reset the currentTracer so its empty for the next try
                currentTracer->Reset();


            }while(returnValue == "restart");

        }
    }

    return 0;
}
