#include "../../include/game/Game.hpp"
#include "../../include/game/Level.hpp"
#include "../../include/game/Settings.hpp"
#include "../../include/game/DialogScreen.hpp"
#include "../../include/game/HighscoreUploader.hpp"
#include "../../include/game/HighscoreBoard.hpp"
#include "../../include/game/Tracer.hpp"
#include "../../include/aw/utilities/Converter.hpp"


#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <cmath> // sqrt

Game::Game(sf::RenderWindow &window) : m_window(window)
{
    m_position = sf::Vector2f(100,100);
    m_lastFrameTime = 0;

    m_running = true;
    m_returnValue = "menu";
}

Game::~Game()
{
    m_pLevel.release();
}

std::string Game::Run(std::string levelName, Tracer &currentTracer, Tracer &lastTracer)
{
    m_levelName = levelName;

    Init();

    m_gameTime.restart();

    SetWindowName();

    if(settings::IsMusicOn())
    {
        m_music.setVolume(settings::GetMusicVolume());
        m_music.play();
    }

    bool start = false;

    while(m_window.isOpen() && m_running && !start)
    {
        sf::Event e;

        while(m_window.pollEvent(e))
        {
            switch(e.type)
            {
            case sf::Event::Closed:
                m_running = false;
                m_returnValue = "exit";
                break;
            case sf::Event::KeyPressed:
                start = true;
                break;
            default:
                break;
            }
        }

        m_window.clear();

        m_pLevel->Draw(m_view.getCenter().x);

        sf::RectangleShape fade;
        fade.setSize(sf::Vector2f(m_window.getSize().x, m_window.getSize().y));
        fade.setFillColor(sf::Color(0,0,0,180));

        sf::Text temp;
        temp.setFont(m_font);
        temp.setString("Press any key to start the game!");
        temp.setPosition(60, 175);
        temp.setCharacterSize(25);

        m_window.draw(fade);
        m_window.draw(temp);

        m_window.display();
    }

    m_frameTime.restart();
    m_gameTime.restart();

    while(m_window.isOpen() && m_running)
    {
        HandleEvents();

        DoLogic(currentTracer);

        if(m_pLevel->GetFinished())
            Finish();

        Draw(currentTracer, lastTracer);

        SetWindowName();
    }

    m_window.setTitle("Kroniax");

    return m_returnValue;
}

void Game::HandleEvents()
{
    sf::Event e;

    while(m_window.pollEvent(e))
    {
        if(e.type == sf::Event::Closed)
            m_window.close();

        if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
            m_running = false;

        if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::P)
        {
            PauseTheGame();
        }

        if(e.type == sf::Event::Resized)
        {
            int width = e.size.width;
            int height = width / 16 * 9;
            m_window.setSize(sf::Vector2u(width, height));

            PauseTheGame();
        }

        if(e.type == sf::Event::KeyPressed && (e.key.code == sf::Keyboard::LAlt || e.key.code == sf::Keyboard::RAlt || e.key.code == sf::Keyboard::Home))
        {
            PauseTheGame();
        }

        if(e.type == sf::Event::LostFocus)
        {
            PauseTheGame();
        }

    }

    if(settings::GetGamemode() == 1)
    {
        if(settings::GetSpeedX() < 1250)
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                settings::SetSpeedX(settings::GetSpeedX() + (150*m_lastFrameTime));
            }
        }

        if(settings::GetSpeedX() > -1250)
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                settings::SetSpeedX(settings::GetSpeedX() - (150*m_lastFrameTime));

            }
        }
    }


}



void Game::DoLogic(Tracer &currentTracer)
{

    m_pLevel->CheckForScripts(m_position.x+20);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        float newValue = settings::GetSpeedY() - (2.3f*settings::GetDownforce() * m_lastFrameTime);
        settings::SetSpeedY(newValue);
    }

    settings::SetSpeedY(settings::GetSpeedY() + settings::GetDownforce()*m_lastFrameTime);

    m_position.x += settings::GetSpeedX() * m_lastFrameTime;
    m_position.y += settings::GetSpeedY() * m_lastFrameTime;

    m_viewPositionFloat = sf::Vector2f(m_viewPositionFloat.x + (settings::GetSpeedX() * m_lastFrameTime), m_viewPositionFloat.y);
    m_view.setCenter(static_cast<int>(m_viewPositionFloat.x), m_viewPositionFloat.y);

    //m_view.setCenter(m_viewPositionFloat);
    m_window.setView(m_view);



    //Time things
    m_time.setPosition(5,5);
    m_speedX.setPosition(150,5);
    if(m_running)
    {
        m_time.setString("Time: "+aw::conv::ToString(aw::conv::ToInt(m_gameTime.GetTimeInMs()/1000.f)));
        m_speedX.setString("SpeedX: " + aw::conv::ToString(aw::conv::ToInt(settings::GetSpeedX())));
    }


    // Check if there current Tracer need an update
    // Multiply with 1000 because this function need milliseconds and not seconds
    if(currentTracer.Update(m_lastFrameTime*1000))
    {
        bool steerUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

        bool speedUp   = false;
        bool speedDown = false;
        //Only check for speedUps and Downs if the gamemode is Speed Challenge
        if(settings::GetGamemode() == 1)
        {
            speedUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
            speedDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        }

        currentTracer.AddPoint(m_position, steerUp, speedUp, speedDown);
    }

}

void Game::Draw(Tracer &currentTracer, Tracer &lastTracer)
{
    m_window.clear(sf::Color::Black);

    m_pLevel->Draw(m_view.getCenter().x);

    //Draw the tracerlines
    lastTracer.Draw(m_window);
    currentTracer.Draw(m_window);


    CreateBody();
    m_window.draw(m_body, 3, sf::Triangles);

    m_window.setView(m_window.getDefaultView());

    m_window.draw(m_time);

    if(settings::GetGamemode() == 1)
    {
        m_window.draw(m_speedX);
    }
    m_window.display();

    //Collision
    if(m_pLevel->Collide(m_view.getCenter().x, sf::Vector2i(m_body[0].position.x, m_body[0].position.y),
                              sf::Vector2i(m_body[1].position.x, m_body[1].position.y),
                              sf::Vector2i(m_body[2].position.x, m_body[2].position.y)))
    {
        if(db::DialogYesNo(m_window, "You hit the wall!\nDo you want to restart this level?"))
        {
            m_returnValue = "restart";
            m_running = false;
        }
        else
        {
            m_returnValue = "menu";
            m_running = false;
        }
    }


    m_lastFrameTime = m_frameTime.getElapsedTime().asSeconds();
    m_frameTime.restart();
}


void Game::Init()
{
    m_pLevel = std::unique_ptr<Level>(new Level(m_window));

    if(!m_pLevel->Load(m_levelName))
    {
        db::DialogYesNo(m_window , "Couldn´t load the Level!\nMaybe name is wrong?");
        m_running = false;
    }

    if(settings::IsMusicOn())
    {
        if(settings::GetMusic() == 1)
        {
            m_music.openFromFile("data/audio/Galaxy - New Electro House Techno by MafiaFLairBeatz.ogg");
        }
        else if(settings::GetMusic() == 2)
        {
            m_music.openFromFile("data/audio/Infinity - Techno Trance Project 2011 by MafiaFLairBeatz.ogg");
        }
        else
        {
            m_music.openFromFile("data/audio/Return of the Electro by MafiaFLairBeatz.ogg");
        }
    }

    sf::Vector2u sizeWin = m_window.getSize();

    m_view = m_window.getView();
    m_view.setCenter(400, 225);
    m_view.setSize(sf::Vector2f(800,450));
    m_viewPositionFloat = m_view.getCenter();

    m_window.setView(m_view);

    m_window.setSize(sizeWin);

    m_font.loadFromFile("data/font/terminator real nfi.ttf");

    m_time.setFont(m_font);
    m_time.setPosition(m_window.mapPixelToCoords(sf::Vector2i(5,5)));
    m_time.setCharacterSize(12);
    m_time.setString("Time: 0");

    m_speedX.setFont(m_font);
    m_speedX.setPosition(m_window.mapPixelToCoords(sf::Vector2i(5, 40)));
    m_speedX.setCharacterSize(12);
    m_speedX.setString("SpeedX: " + aw::conv::ToString(aw::conv::ToInt(settings::GetSpeedX())));

    settings::SetSpeedX(settings::GetStartSpeedX());

    CreateBody();
}


void Game::PauseTheGame()
{
    m_gameTime.pause();
    db::DialogOK(m_window, "Game Paused! Press return/enter to continue");
    m_gameTime.resume();
    m_frameTime.restart();
}


void Game::Finish()
{

    if(settings::GetGamemode() == 0)
    {

        int position = settings::GetUnlockedLevel() -1;
        if(position < 0)
            position = 0;

        if(m_levelName == settings::GetLevelList().at(position))
        {
            if(settings::IncreaseUnlockLevel())
            {
                db::DialogOK(m_window, "You unlocked a new Map!");
                //To save the current level progress
                settings::Save();
            }
        }



        db::DialogOK(m_window, "Congratulation, you completed\nthis Level!");

        m_running = false;
        m_returnValue = "win";
    }

    else if(settings::GetGamemode() == 1)
    {
        m_gameTime.pause();

        if(db::DialogYesNo(m_window, "Do you want to upload your time?\nInternetconnection is needed"))
        {
            HighscoreUploader uploader;



            std::string name = db::InputDialog(m_window, "Please insert your name: ", "guest");

            //Return false = The connection failed
            if(!uploader.Submit(m_levelName, name, m_pLevel->GetLevelLength(), m_pLevel->GetFilledBlocks(), m_pLevel->GetCollisionBlocks(), m_gameTime.GetTimeInMs()))
            {
                // Give the user feedback about the failed Connection
                db::DialogOK(m_window, uploader.GetError());
            }
            else
            {
                //If the connection works get the Highscore
                std::vector<Score> &temp = uploader.GetScore();
                // And display them
                ListHighscore(m_window, m_levelName, temp);
            }

            if(db::DialogYesNo(m_window, "Do you want to play this level again?"))
            {
                m_returnValue = "restart";
            }
            else
            {
                m_returnValue = "win";
            }
            m_running = false;
        }
    }

}

void Game::CreateBody()
{
    float height = 7;
    float width  = 25;

    sf::Vector2f dVec; // unitVector of v
    double magSpeed = std::sqrt((settings::GetSpeedX() * settings::GetSpeedX()) + (settings::GetSpeedY() * settings::GetSpeedY()));
    dVec.x = settings::GetSpeedX() / magSpeed;
    dVec.y = settings::GetSpeedY() / magSpeed;

    m_body[2] = sf::Vertex(sf::Vector2f(m_position.x + (width * dVec.x), m_position.y + (width * dVec.y)), sf::Color(255,255,255)); // Right

    // normal Vector from the unint V vector
    sf::Vector2f dnVec;
    dnVec.x = dVec.y;
    dnVec.y = -dVec.x;

    m_body[0] = sf::Vertex(sf::Vector2f(m_position.x + height * dnVec.x, m_position.y + height * dnVec.y), sf::Color(255,255,255)); // Left Bot
    m_body[1] = sf::Vertex(sf::Vector2f(m_position.x - height * dnVec.x, m_position.y - height * dnVec.y), sf::Color(255,255,255)); // Left Top

}


void Game::SetWindowName()
{
    float progress = m_position.x / (m_pLevel->GetLevelLength()*m_pLevel->GetTileSize().x);
    int iProgress = static_cast<int>(progress*10);

    int i = 0;
    std::string title = m_levelName+ "Progress: ";

    while(i < 10)
    {
        if(i == iProgress)
            title = title + ">";
        else
            title = title + "-";

        i++;
    }

    m_window.setTitle(title);

}
