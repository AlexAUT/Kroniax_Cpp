#include "../../include/game/Menu.hpp"
#include "../../include/aw/utilities/Converter.hpp"
#include "../../include/game/HighscoreUploader.hpp"
#include "../../include/game/HighscoreBoard.hpp"
#include "../../include/game/Settings.hpp"
#include "../../include/game/MenuBackground.hpp"
#include "../../include/game/DialogScreen.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>

#include <fstream>

Menu::Menu(sf::RenderWindow &window) : m_window(window), m_running(true), m_returnValue("exit")
{
    m_pbg = nullptr;
    m_pbg = new MenuBackground(window);
}

Menu::~Menu()
{
    if(m_pbg)
        delete m_pbg;

    m_pbg = nullptr;
}



std::string Menu::Run()
{
    Init();

    while(m_window.isOpen() && m_running)
    {
        HandleEvents();

        Draw();
    }

    m_music.pause();

    if(m_pbg)
        delete m_pbg;

    m_pbg = nullptr;

    return m_returnValue;
}



void Menu::Init()
{
    m_window.setView(sf::View(sf::Vector2f(400,225), sf::Vector2f(800,450)));

    m_texHeadline.loadFromFile("data/sprites/kroniax.png");
    m_srHeadline.setTexture(m_texHeadline);
    m_srHeadline.setScale(0.7f,0.7f);

    m_srHeadline.setPosition(160,-50);

    m_pbg->Init();


    m_music.openFromFile("data/audio/Power Fight - Electro Techno Beat.ogg");

    m_music.setVolume(settings::GetMusicVolume());
    m_music.setLoop(true);

    if(!settings::IsMusicOn())
        m_music.setVolume(0);
    m_music.play();

    m_underlay.setPosition(220,150);
    m_underlay.setSize(sf::Vector2f(350,260));
    m_underlay.setFillColor(sf::Color(154,154,154,154));

    InitGui();
}

void Menu::HandleEvents()
{
    sf::Event e;

    while(m_window.pollEvent(e))
    {
        if(e.type == sf::Event::Closed)
        {
            m_running = false;
            m_returnValue = "exit";
        }

        if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
        {
            m_running = false;
            m_returnValue = "exit";
        }

        m_gui.HandleEvents(e);


        if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Return)
        {
            if(m_gui.GetSelectedElement()->GetID() == "arcade")
            {
                m_gui.SetActiveLayer(4);
            }

            else if(m_gui.GetSelectedElement()->GetID() == "speed challenge")
            {
                m_gui.SetActiveLayer(5);
            }

            else if(m_gui.GetSelectedElement()->GetID() == "custom Map")
            {
                m_gui.SetActiveLayer(1);
            }

            else if(m_gui.GetSelectedElement()->GetID() == "start")
            {
                StartGame(m_gui.GetSelectedLayer()->GetElement(0)->GetText());
            }

            else if(m_gui.GetSelectedElement()->GetID() == "exit")
            {
                settings::Save();
                m_running = false;
                m_returnValue = "exit";
            }

            else if(m_gui.GetSelectedElement()->GetID() == "credits")
            {
                m_gui.SetActiveLayer(2);
                m_gui.GetSelectedLayer()->SetActiveElement(3);
            }

            else if(m_gui.GetSelectedElement()->GetID() == "options")
            {
                m_gui.SetActiveLayer(3);
                m_gui.GetSelectedLayer()->SetActiveElement(1);
            }

            else if(m_gui.GetSelectedElement()->GetID() == "back")
            {

                // If the game returns from the option menu save the new options.
                if(m_gui.GetActiveLayerInt() == 3)
                {
                     settings::AdjustVolume(aw::conv::ToInt(m_gui.GetSelectedLayer()->GetElement(3)->GetText()));

                    if(m_gui.GetSelectedLayer()->GetElement(1)->GetText() == "on")
                    {
                        settings::EnableMusic(true);
                    }
                    else
                    {
                        settings::EnableMusic(false);
                    }

                    m_music.setVolume(settings::GetMusicVolume());

                    if(!settings::IsMusicOn())
                        m_music.setVolume(0);

                    //Save the new Antialiasing level
                    settings::SetAntialiasing(aw::conv::ToInt(m_gui.GetSelectedLayer()->GetElement(5)->GetText()));
                    //Has the anitailiasing level change? -> Yes create a new window with the new settings
                    if(m_window.getSettings().antialiasingLevel != settings::GetAntialiasing())
                    {
                        //The new Window should have the same settings except the antia. level
                        sf::ContextSettings newSettings = m_window.getSettings();
                        newSettings.antialiasingLevel = settings::GetAntialiasing();
                        //Store the old size
                        //The size has to set after the creating because otherwise the dialogscreens
                        //And the interface during the game will be display not correctly!
                        //So you have to create the window with the size (800,450)
                        //Because of the defaultview.
                        sf::Vector2u oldSize = m_window.getSize();
                        //Store to old view to set it for the "new" window
                        sf::View currentView = m_window.getView();
                        //Store the old position of the window
                        sf::Vector2i oldPos = m_window.getPosition();
                        //Create a new window with the same size but other contextsettings.
                        //The size has to set after the creating because otherwise the dialogscreens
                        //And the interface during the game will be display not correctly!
                        m_window.create(sf::VideoMode(800,450), "Kroniax", sf::Style::Default, newSettings);
                        //Set the new Size
                        m_window.setSize(oldSize);
                        //Set the old view
                        m_window.setView(currentView);
                        //Set the new Position
                        m_window.setPosition(oldPos);

                    }

                    //Save the new times for the tracer
                    settings::SetTimeForTracer(aw::conv::ToInt(m_gui.GetSelectedLayer()->GetElement(7)->GetText()));

                    //Save new settings into the config file
                    settings::Save();

                }

                m_gui.SetActiveLayer(0);

            }

            else if(m_gui.GetSelectedElement()->GetID() == "startArcade")
            {
                std::string lvlname = m_gui.GetSelectedLayer()->GetElement(0)->GetText();
                settings::SetGamemode(0);
                StartGame(lvlname);
            }

            else if(m_gui.GetSelectedElement()->GetID() == "lvl")
            {
                std::string lvlname = m_gui.GetSelectedLayer()->GetElement(0)->GetText();
                settings::SetGamemode(0);
                StartGame(lvlname);
            }


            else if(m_gui.GetSelectedElement()->GetID() == "helparcade")
            {
                db::DialogOK(m_window, "Complete the newest Level\nto unlock more levels");
            }

            else if(m_gui.GetSelectedElement()->GetID() == "helpSpeed")
            {
                db::DialogOK(m_window, "You can play the levels you have unlocked\n in the arcade mode\n\nSpeed ups and slow downs are disabled.\n\nYou can control your speed with the arrowkeys.\n\nYou cant influence the gravitation!\nGrav ups and down are still working.\n\nThere is an online highscore where you\n can compare your time with others!");
            }

            else if(m_gui.GetSelectedElement()->GetID() == "getHighscore")
            {
                std::string lvlname1 = db::InputDialog(m_window, "Enter the name of the Level", "Level1");

                HighscoreUploader uploader;

                //Returns false if the connection failed
                if(!uploader.GetHighscore(lvlname1))
                {
                    //Give the user feedback about the error
                    db::DialogOK(m_window, uploader.GetError());
                }
                else
                {
                    //List the Highscore if the connection worked
                    ListHighscore(m_window, lvlname1, uploader.GetScore());
                }


            }

            else if(m_gui.GetSelectedElement()->GetID() == "startSpeed")
            {
                std::string lvlname = m_gui.GetSelectedLayer()->GetElement(0)->GetText();
                settings::SetGamemode(1);
                StartGame(lvlname);
            }

              else if(m_gui.GetSelectedElement()->GetID() == "lvl speed")
            {
                std::string lvlname = m_gui.GetSelectedLayer()->GetElement(0)->GetText();
                settings::SetGamemode(1);
                StartGame(lvlname);
            }

        }
    }

}

void Menu::Draw()
{


    m_pbg->DoLogic();

    m_window.clear();

    m_pbg->Draw();

    m_window.draw(m_underlay);

    m_gui.Draw(m_window);

    m_window.draw(m_srHeadline);

    m_window.display();
}

std::string Menu::GetLevelName()
{
    return m_levelName;
}


void Menu::InitGui()
{
    m_gui.AddLayer();

    m_gui.AddElement(0, 1, "arcade", sf::Vector2f(325,150), "Arcade");

    m_gui.AddElement(0, 1, "speed challenge", sf::Vector2f(250,185), "Speed Challenge");

    m_gui.AddElement(0, 1, "custom Map", sf::Vector2f(300,220), "Custom Map");

    m_gui.AddElement(0, 1, "options", sf::Vector2f(325,270), "Options");

    m_gui.AddElement(0, 1, "credits", sf::Vector2f(327,310), "Credits");

    m_gui.AddElement(0, 1, "exit", sf::Vector2f(355,360), "Exit");

///////////////////////////////////////////////////////////////

    m_gui.AddLayer();

    m_gui.AddElement(1, 3, "name", sf::Vector2f(315,160), "Levelname");

    m_gui.AddElement(1, 1, "start", sf::Vector2f(350,220), "Start");

    m_gui.AddElement(1, 1, "back", sf::Vector2f(355,360), "Back");

//////////////////////////////////////////////////////////////////////////////

    m_gui.AddLayer();

    m_gui.AddElement(2, 0, "0", sf::Vector2f(50,180), "Music by MafiaFLairBeatz");
    m_gui.GetLastLayer()->GetLastElement()->SetSelectAble(false);

    m_gui.AddElement(2, 0, "0", sf::Vector2f(50,220), "The great library SFML created by Laurent");
    m_gui.GetLastLayer()->GetLastElement()->SetSelectAble(false);

    m_gui.AddElement(2, 0, "0", sf::Vector2f(50,255), "Developed by Alexander Weinrauch");
    m_gui.GetLastLayer()->GetLastElement()->SetSelectAble(false);

    m_gui.AddElement(2, 1, "back", sf::Vector2f(355,360), "Back");

///////////////////////////////////////////////////////////////////////

    m_gui.AddLayer();

    m_gui.AddElement(3, 1, "music", sf::Vector2f(280,180), "Music: ");
    m_gui.GetLastLayer()->GetLastElement()->SetSelectAble(false);


    m_gui.AddElement(3, 2, "vol", sf::Vector2f(500,180), "on");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("on");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("off");

    if(settings::IsMusicOn())
    {
        m_gui.GetLastLayer()->GetLastElement()->SetActiveEntry(0);
    }
    else
    {
        m_gui.GetLastLayer()->GetLastElement()->SetActiveEntry(1);
    }



    m_gui.AddElement(3, 1, "music", sf::Vector2f(280,220), "Musicvoume: ");
    m_gui.GetLastLayer()->GetLastElement()->SetSelectAble(false);


    m_gui.AddElement(3, 2, "vol", sf::Vector2f(500,220), "50");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("0");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("10");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("20");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("30");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("40");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("50");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("60");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("70");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("80");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("90");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("100");

    int index = settings::GetMusicVolume() / 10;

    m_gui.GetLastLayer()->GetLastElement()->SetActiveEntry(index);


    // Antialiasing option
    m_gui.AddElement(3, 1, "anti", sf::Vector2f(280,260), "Antialiasing: ");
    m_gui.GetLastLayer()->GetLastElement()->SetSelectAble(false);


    m_gui.AddElement(3, 2, "antialiasing", sf::Vector2f(505,260), "0");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("0");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("2");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("4");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("8");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("16");


    switch(settings::GetAntialiasing())
    {
    case 2: index = 1; break;
    case 4: index = 2; break;
    case 8: index = 3; break;
    case 16: index = 4; break;
    default : index = 0; break;
    }
    m_gui.GetLastLayer()->GetLastElement()->SetActiveEntry(index);


    // TimeBetweenPoints option
    m_gui.AddElement(3, 1, "tracertim", sf::Vector2f(280,300), "Time between \ntracer points: ");
    m_gui.GetLastLayer()->GetLastElement()->SetCharacterSize(14);
    m_gui.GetLastLayer()->GetLastElement()->SetSelectAble(false);


    m_gui.AddElement(3, 2, "tracertime", sf::Vector2f(480,300), "800");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("75");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("100");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("300");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("600");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("800");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("1000");


    switch(settings::GetTimeForTracer())
    {
    case 75:   index = 0; break;
    case 100:  index = 1; break;
    case 300:  index = 2; break;
    case 600:  index = 3; break;
    case 800: index = 4; break;
    default :  index = 5; break;
    }
    m_gui.GetLastLayer()->GetLastElement()->SetActiveEntry(index);


    m_gui.AddElement(3, 1, "back", sf::Vector2f(355,360), "Back");


    //////////////////////////////////////////////////////////////////


    m_gui.AddLayer();// 5 Arcade Mode


    m_gui.AddElement(4, 2, "lvl", sf::Vector2f(335,150), "Select a level");


    for(int i = 0; i < settings::GetUnlockedLevel(); i++)
    {
        m_gui.GetLastLayer()->GetLastElement()->AddEntry(settings::GetLevelList().at(i));
    }

    if(settings::GetUnlockedLevel() > 1)
    {
        m_gui.GetLastLayer()->GetLastElement()->SetActiveEntry(settings::GetUnlockedLevel()-1);
    }

    m_gui.AddElement(4, 1, "startArcade", sf::Vector2f(345,220), "Start");

    m_gui.AddElement(4, 1, "helparcade", sf::Vector2f(355,255), "Help");


    m_gui.AddElement(4, 1, "back", sf::Vector2f(355,360), "Back");




    m_gui.AddLayer();// 6 Speedchallenge


    m_gui.AddElement(5, 2, "lvl speed", sf::Vector2f(335,150), "Select a level for Speedchallenge");



    for(int i = 0; i < settings::GetUnlockedLevel(); i++)
    {
        m_gui.GetLastLayer()->GetLastElement()->AddEntry(settings::GetLevelList().at(i));
    }

    if(settings::GetUnlockedLevel() > 1)
    {
        m_gui.GetLastLayer()->GetLastElement()->SetActiveEntry(settings::GetUnlockedLevel()-1);
    }

    m_gui.AddElement(5, 1, "startSpeed", sf::Vector2f(345,220), "Start");

    m_gui.AddElement(5, 1, "helpSpeed", sf::Vector2f(355,255), "Help");

    m_gui.AddElement(5, 1, "getHighscore", sf::Vector2f(250, 290), "Show Highscore");


    m_gui.AddElement(5, 1, "back", sf::Vector2f(355,360), "Back");

}



void Menu::StartGame(std::string lvlname)
{
    std::fstream file;
    file.open(("levels/"+lvlname+"/settings.ini").c_str(), std::ios::in);

    if(file.fail())
    {
        db::DialogOK(m_window , "Couldnt find the level Data.\nPlease try it again");
        return;
    }

    std::string line;

    std::string gameMode;
    std::string difficulty;

    while(!file.eof())
    {
        std::getline(file, line);

        if(line == "[Difficulty]")
        {
            file.eof();
            std::getline(file, line);

            difficulty = line;
        }

        if(line == "[SpeedX]")
        {
            file.eof();
            std::getline(file, line);

            settings::SetSpeedX(aw::conv::ToFloat(line));
            settings::SetStartSpeedX(aw::conv::ToFloat(line));
        }

        if(line == "[Downforce]")
        {
            file.eof();
            std::getline(file, line);

            settings::SetStartDownforce(aw::conv::ToFloat(line));
            settings::SetDownforce(aw::conv::ToFloat(line));
        }
    }

    if(db::DialogYesNo(m_window, ("Do you want to play this level?\n\n\nName: "+lvlname+"\nDifficulty: "+difficulty)))
    {
        m_running = false;
        m_returnValue = "game";
        m_levelName = lvlname;

        if(gameMode == "Normal")
            settings::SetGamemode(0);
        else if(gameMode == "Speed challenge")
            settings::SetGamemode(1);
    }
}
