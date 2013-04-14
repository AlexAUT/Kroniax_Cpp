#include "../../include/game/HighscoreBoard.hpp"
#include "../../include/aw/utilities/Converter.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>


void ListHighscore(sf::RenderWindow &window, std::string lvlname, std::vector<Score> &m_scores)
{
    bool running = true;

    float startX = window.getView().getCenter().x - window.getView().getSize().x/2;

    sf::Font font;
    font.loadFromFile("data/font/terminator real nfi.ttf");

    std::string headlineText = "Highscore of " + lvlname;

    sf::Text headline;
    headline.setFont(font);
    headline.setString(headlineText);
    headline.setPosition(startX + 150, 25);

    sf::Text howTo;
    howTo.setFont(font);
    howTo.setString("Press enter to continue");
    howTo.setCharacterSize(10);
    howTo.setPosition(startX + 250,430);


    sf::Text m_times[10];
    sf::Text m_names[10];

    for(unsigned int i = 0; i < m_scores.size(); i++)
    {
        m_times[i].setFont(font);
        m_times[i].setCharacterSize(15);
        m_times[i].setString(aw::conv::ToString(m_scores[i].score / 1000.f) + "  secs");
        m_times[i].setPosition(startX + 150, 100 + (30*i));

        m_names[i].setFont(font);
        m_names[i].setCharacterSize(15);
        m_names[i].setString(m_scores[i].name);
        m_names[i].setPosition(startX + 450, 100 + (30*i));
    }


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


    while(running && window.isOpen())
    {
        sf::Event e;
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                window.close();

            if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Return)
                running = false;
        }

        window.clear();


        window.draw(oldScreenSprite);
        window.draw(overlay);

        window.draw(headline);
        window.draw(howTo);

        for(unsigned int i = 0; i < m_scores.size(); i++)
        {
            window.draw(m_times[i]);
            window.draw(m_names[i]);
        }

        window.display();
    }
}
