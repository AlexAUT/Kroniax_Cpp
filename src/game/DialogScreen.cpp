#include "../../include/game/DialogScreen.hpp"
#include "../../include/gui/GuiController.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

namespace db
{

void DialogOK(sf::RenderWindow &window, std::string text)
{
    GuiController gui;


    gui.AddLayer();


    gui.AddLabel(0, "text", sf::Vector2f(10,10), text);
    gui.GetLastLayer()->GetLastElement()->SetCharacterSize(15);
    gui.GetLastLayer()->GetLastElement()->SetSelectable(false);

    float startX = window.getView().getCenter().x - window.getView().getSize().x/2;

    //Calculate Position
    sf::Vector2f newPos = sf::Vector2f(startX +(window.getView().getSize().x / 2) - (gui.GetLastLayer()->GetLastElement()->GetTextObj().getLocalBounds().width/2),
                                (window.getView().getSize().y / 2) - (gui.GetLastLayer()->GetLastElement()->GetTextObj().getLocalBounds().height));


    gui.GetLastLayer()->GetLastElement()->SetPosition(newPos);


   gui.AddButton(0, "ok", sf::Vector2f(10,10), "OK");


     newPos = sf::Vector2f(startX +(window.getView().getSize().x / 2) - (gui.GetLastLayer()->GetLastElement()->GetTextObj().getLocalBounds().width / 2),
                          50 + (window.getView().getSize().y/2 + 10));

    gui.GetLastLayer()->GetLastElement()->SetPosition(newPos);


    gui.GetLastLayer()->SetActiveElement(1);


    sf::Image oldScreen = window.capture();
    sf::Texture oldTex;
    oldTex.loadFromImage(oldScreen);
    sf::Sprite oldScreenSprite;
    oldScreenSprite.setTexture(oldTex);

    float scaleX = window.getView().getSize().x / oldScreenSprite.getLocalBounds().width;
    float scaleY = window.getView().getSize().y / oldScreenSprite.getLocalBounds().height;

    oldScreenSprite.setPosition(window.mapPixelToCoords(sf::Vector2i(0,0)));
    oldScreenSprite.setScale(scaleX, scaleY);


    sf::RectangleShape overlay;
    overlay.setPosition(window.mapPixelToCoords(sf::Vector2i(0,0)));
    overlay.setSize(sf::Vector2f(window.getView().getSize().x, window.getView().getSize().y));
    overlay.setFillColor(sf::Color(0,0,0,210));

    while(window.isOpen())
    {
        sf::Event e;

        while(window.pollEvent(e))
        {
            gui.HandleEvent(e);

            if(e.type == sf::Event::Closed)
                window.close();

            if(e.type == sf::Event::KeyPressed)
            {
                if(e.key.code == sf::Keyboard::Escape)
                    window.close();

                if(e.key.code == sf::Keyboard::Return)
                {
                    if(gui.GetSelectedElement()->GetID() == "ok")
                    {
                        return;
                    }
                }
            }


        }

        window.draw(oldScreenSprite);
        window.draw(overlay);
        gui.Draw(window);

        window.display();
    }

}



bool DialogYesNo(sf::RenderWindow &window, std::string text)
{
    GuiController gui;

    gui.AddLayer();


    gui.AddLabel(0, "text", sf::Vector2f(10,10), text);
    gui.GetLastLayer()->GetLastElement()->SetCharacterSize(15);

    float startX = window.getView().getCenter().x - window.getView().getSize().x/2;

    //Calculate Position
    sf::Vector2f newPos = sf::Vector2f(startX +(window.getView().getSize().x / 2) - (gui.GetLastLayer()->GetLastElement()->GetTextObj().getLocalBounds().width/2),
                                (window.getView().getSize().y / 2) - (gui.GetLastLayer()->GetLastElement()->GetTextObj().getLocalBounds().height));


    gui.GetLastLayer()->GetLastElement()->SetPosition(newPos);

    gui.AddButton(0, "yes", sf::Vector2f(10,10), "Yes");


    newPos = sf::Vector2f(startX +(window.getView().getSize().x / 2) - (gui.GetLastLayer()->GetLastElement()->GetTextObj().getLocalBounds().width / 2),
                          50 + (window.getView().getSize().y/2 + 10));

    gui.GetLastLayer()->GetLastElement()->SetPosition(newPos);


    gui.AddButton(0, "no", sf::Vector2f(10,10), "No");

    newPos = sf::Vector2f(startX +(window.getView().getSize().x / 2) - (gui.GetLastLayer()->GetLastElement()->GetTextObj().getLocalBounds().width / 2),
                          50 + (window.getView().getSize().y/2 + 40));

    gui.GetLastLayer()->GetLastElement()->SetPosition(newPos);


    gui.GetLastLayer()->SetActiveElement(1);

    sf::Image oldScreen = window.capture();
    sf::Texture oldTex;
    oldTex.loadFromImage(oldScreen);
    sf::Sprite oldScreenSprite;
    oldScreenSprite.setTexture(oldTex);

    float scaleX = window.getView().getSize().x / oldScreenSprite.getLocalBounds().width;
    float scaleY = window.getView().getSize().y / oldScreenSprite.getLocalBounds().height;

    oldScreenSprite.setPosition(window.mapPixelToCoords(sf::Vector2i(0,0)));
    oldScreenSprite.setScale(scaleX, scaleY);


    sf::RectangleShape overlay;
    overlay.setPosition(window.mapPixelToCoords(sf::Vector2i(0,0)));
    overlay.setSize(sf::Vector2f(window.getView().getSize().x, window.getView().getSize().y));
    overlay.setFillColor(sf::Color(0,0,0,210));

    while(window.isOpen())
    {
        sf::Event e;

        while(window.pollEvent(e))
        {
            gui.HandleEvent(e);

            if(e.type == sf::Event::Closed)
                window.close();

            if(e.type == sf::Event::KeyPressed)
            {
                if(e.key.code == sf::Keyboard::Escape)
                    window.close();

                if(e.key.code == sf::Keyboard::Return)
                {
                    if(gui.GetSelectedElement()->GetID() == "yes")
                    {
                        return true;
                    }
                    if(gui.GetSelectedElement()->GetID() == "no")
                    {
                        return false;
                    }
                }
            }


        }

        window.clear();

        window.draw(oldScreenSprite);
        window.draw(overlay);
        gui.Draw(window);

        window.display();
    }

    return false;
}

std::string InputDialog(sf::RenderWindow &window, std::string text, std::string inputText)
{
    GuiController gui;

    gui.AddLayer();


    gui.AddLabel(0, "text", sf::Vector2f(10,10), text);
    gui.GetLastLayer()->GetLastElement()->SetCharacterSize(15);

    float startX = window.getView().getCenter().x - window.getView().getSize().x/2;

    //Calculate Position
    sf::Vector2f newPos = sf::Vector2f(startX +(window.getView().getSize().x / 2) - (gui.GetLastLayer()->GetLastElement()->GetTextObj().getLocalBounds().width/2),
                                (window.getView().getSize().y / 2) - (gui.GetLastLayer()->GetLastElement()->GetTextObj().getLocalBounds().height + 50));


    gui.GetLastLayer()->GetLastElement()->SetPosition(newPos);

   gui.AddInput(0, "inp", sf::Vector2f(10,10), inputText);


    newPos.y = (window.getView().getSize().y/2 + 10);

    gui.GetLastLayer()->GetLastElement()->SetPosition(newPos);


    gui.AddButton(0, "submit", sf::Vector2f(10,10), "Submit");

    newPos = sf::Vector2f(startX +(window.getView().getSize().x / 2) - (gui.GetLastLayer()->GetLastElement()->GetTextObj().getLocalBounds().width / 2),
                          (window.getView().getSize().y/2 + 75));

    gui.GetLastLayer()->GetLastElement()->SetPosition(newPos);


    gui.GetLastLayer()->SetActiveElement(1);

    sf::Image oldScreen = window.capture();
    sf::Texture oldTex;
    oldTex.loadFromImage(oldScreen);
    sf::Sprite oldScreenSprite;
    oldScreenSprite.setTexture(oldTex);

    float scaleX = window.getView().getSize().x / oldScreenSprite.getLocalBounds().width;
    float scaleY = window.getView().getSize().y / oldScreenSprite.getLocalBounds().height;

    oldScreenSprite.setPosition(window.mapPixelToCoords(sf::Vector2i(0,0)));
    oldScreenSprite.setScale(scaleX, scaleY);


    sf::RectangleShape overlay;
    overlay.setPosition(window.mapPixelToCoords(sf::Vector2i(0,0)));
    overlay.setSize(sf::Vector2f(window.getView().getSize().x, window.getView().getSize().y));
    overlay.setFillColor(sf::Color(0,0,0,210));

    while(window.isOpen())
    {
        sf::Event e;

        while(window.pollEvent(e))
        {
            gui.HandleEvent(e);

            if(e.type == sf::Event::Closed)
                window.close();

            if(e.type == sf::Event::KeyPressed)
            {
                if(e.key.code == sf::Keyboard::Escape)
                    window.close();

                if(e.key.code == sf::Keyboard::Return)
                {
                    if(gui.GetSelectedElement()->GetID() == "submit")
                    {
                        return gui.GetLastLayer()->GetElement(1)->GetText();
                    }
                }
            }


        }

        window.draw(oldScreenSprite);
        window.draw(overlay);
        gui.Draw(window);

        window.display();
    }

    return gui.GetLastLayer()->GetElement(1)->GetText();
}


}
