#include <fstream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>

#include "../../include/game/Menu.hpp"
#include "../../include/aw/utilities/Converter.hpp"
#include "../../include/game/HighscoreUploader.hpp"
#include "../../include/game/HighscoreBoard.hpp"
#include "../../include/game/Settings.hpp"
#include "../../include/game/MenuBackground.hpp"
#include "../../include/game/DialogScreen.hpp"


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

        m_gui.HandleEvent(e);


        if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Return)
        {
            //
            // Main Menu
            //
            if(m_gui.GetSelectedElement()->GetID() == "singleplayer")
            {
                m_gui.SetActiveLayer(4);
            }
            else if(m_gui.GetSelectedElement()->GetID() == "multiplayer")
            {
                db::DialogOK(m_window, "In progress");
            }
            else if(m_gui.GetSelectedElement()->GetID() == "help")
            {
                db::DialogOK(m_window, "In progress");
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
            //
            // Back button
            //
            else if(m_gui.GetSelectedElement()->GetID() == "back")
            {

                m_underlay.setPosition(220,150);
                m_underlay.setSize(sf::Vector2f(350,260));

                // If the game returns from the option menu save the new options.
                if(m_gui.GetActiveLayerInt() == 3)
                {
                    //Save the new times for the tracer
                    settings::SetTimeForTracer(aw::conv::ToInt(m_gui.GetSelectedLayer()->GetElement(7)->GetText()));

                    //Save new settings into the config file
                    settings::Save();
                }

                m_gui.SetActiveLayer(0);

            }

            //
            //Singleplayer Layer
            //
            else if(m_gui.GetSelectedElement()->GetID() == "arcade layer")
            {
                m_gui.SetActiveLayer(5);
                settings::SetGamemode(ARCADE);
            }
            else if(m_gui.GetSelectedElement()->GetID() == "speedchallenge layer")
            {
                m_gui.SetActiveLayer(5);
                settings::SetGamemode(SPEED_CHALLENGE);
            }
            //
            // Arcade & Speed Challenge modes
            else if(m_gui.GetSelectedElement()->GetID() == "startArcade")
            {
                std::string lvlname = m_gui.GetSelectedLayer()->GetElement(0)->GetText();
                StartGame(lvlname);
            }

            else if(m_gui.GetSelectedElement()->GetID() == "lvl arcade")
            {
                std::string lvlname = m_gui.GetSelectedLayer()->GetElement(0)->GetText();
                StartGame(lvlname);
            }


            else if(m_gui.GetSelectedElement()->GetID() == "helparcade")
            {
                //Show the right Help depending on the selected gamemode!
                if(settings::GetGamemode() == ARCADE)
                    db::DialogOK(m_window, "Complete the newest Level\nto unlock more levels");
                else if(settings::GetGamemode() == SPEED_CHALLENGE)
                    db::DialogOK(m_window, "In this game mode you cancontrol your speed\nwith the arrow keys\n\n\nSpeed ups and slow downs are disabled\n\n\nIf you complete a level you can upload your time\nto an online highscore\n\n\nComplete the newest Levelin the arcade mode\nto unlock more levels");
            }
            //
            //Custom maps
            //
            else if(m_gui.GetSelectedElement()->GetID() == "custommaps")
            {
                m_gui.SetActiveLayer(6);
                m_underlay.setPosition(150,120);
                m_underlay.setSize(sf::Vector2f(600,325));
            }
            else if(m_gui.GetSelectedElement()->GetID() == "updatelevellist")
            {
                if(m_mapManager.UpdateMapList())
                {
                    m_mapManager.LoadMapList();

                    m_gui.GetElement(6, 0)->ClearEntries();

                    for(unsigned int i = 0; i < m_mapManager.GetMapCount(); i++)
                    {
                        m_gui.GetElement(6, 0)->AddEntry(m_mapManager.GetMap(i)->name);
                    }
                    m_gui.GetElement(6, 0)->SetText("Select a level");

                    db::DialogOK(m_window, "Successfully updated the levellist");
                }
                else
                {
                    db::DialogOK(m_window, "Can´t connect to the server.\nPlease try it again later");
                }
            }
            else if(m_gui.GetSelectedElement()->GetID() == "play")
            {
                if(m_gui.GetSelectedElement()->GetText() == "Download")
                {
                    if(m_mapManager.DownloadMapDescription(m_gui.GetElement(6,0)->GetText()) && m_mapManager.DownloadMap(m_gui.GetElement(6,0)->GetText()))
                    {
                        db::DialogOK(m_window, "Downloaded the map!\nHave fun with it =)");
                    }
                    else
                    {
                        db::DialogOK(m_window, "Couldn't connect to the server\nor couldn't find the map on the server!");
                    }
                }
                else if(m_gui.GetSelectedElement()->GetText() == "Play")
                {
                    Map *currentMap = nullptr;
                    currentMap = m_mapManager.GetMap(m_gui.GetElement(6,0)->GetText());

                    if(currentMap)
                    {
                        if(currentMap->gameMode == "arcade")
                        {
                            settings::SetGamemode(ARCADE);
                            StartGame(m_gui.GetElement(6,0)->GetText());
                        }
                        else if(currentMap->gameMode == "speed challenge")
                        {
                            if(m_mapManager.DownloadMap(currentMap->name))
                            {
                                settings::SetGamemode(SPEED_CHALLENGE);
                                StartGame(currentMap->name);
                            }
                            else
                            {
                                db::DialogOK(m_window, "Can't connect to the server!\nTo play this gamemode a connection is necessary!");
                            }
                        }
                    }
                }
            }
        }
        //
        // Lists Events (All in 1 if to save some performance
        //
        else if (e.type == sf::Event::KeyPressed &&
            (e.key.code == sf::Keyboard::Right || e.key.code == sf::Keyboard::Left))
        {
            if(m_gui.GetSelectedElement()->GetID() == "lvl arcade")
            {
                m_gui.GetSelectedElement()->SetObjectToCenter(m_window.getView().getSize().x);
            }

            if(m_gui.GetSelectedElement()->GetID() == "vol")
            {
                // Apply new music volume setting
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
            }

            else if(m_gui.GetSelectedElement()->GetID() == "antialiasing")
            {
                // Apply new anti-aliasing level
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
                    //Set vert. sync
                    m_window.setVerticalSyncEnabled(true);
                }
            }
        }
    }

}

void Menu::Draw()
{
    UpdateCustomMapScreen();

    m_pbg->DoLogic();

    m_window.clear();

    m_pbg->Draw();

    m_window.draw(m_underlay);

    m_gui.Draw(m_window);

    m_window.draw(m_srHeadline);

    m_window.display();
}
#include <iostream>
void Menu::UpdateCustomMapScreen()
{
    Map* currentMap = nullptr;
    currentMap = m_mapManager.GetMap(m_gui.GetElement(6,0)->GetText());

    if(currentMap)
    {
        m_gui.GetElement(6, 1)->SetText("Author: "+currentMap->author);
        m_gui.GetElement(6, 2)->SetText("Gamemode: "+currentMap->gameMode);
        m_gui.GetElement(6, 3)->SetText("Difficulty: "+currentMap->difficulty);
        m_gui.GetElement(6, 4)->SetText("SpeedX: "+ aw::conv::ToString(currentMap->speedX));
        m_gui.GetElement(6, 5)->SetText("Length: "+ aw::conv::ToString(currentMap->length));

        if(currentMap->speedX != 0)
        {
            m_gui.GetElement(6, 6)->SetText("Play");
        }
        else
        {
            m_gui.GetElement(6, 6)->SetText("Download");
        }
    }



}

std::string Menu::GetLevelName()
{
    return m_levelName;
}


void Menu::InitGui()
{
    m_gui.AddLayer();

    m_gui.AddButton(0, "singleplayer", sf::Vector2f(300,155), "Singleplayer");

    m_gui.AddButton(0, "multiplayer", sf::Vector2f(310,185), "Multiplayer");



    m_gui.AddButton(0, "help", sf::Vector2f(364,260), "Help");

    m_gui.AddButton(0, "options", sf::Vector2f(341,290), "Options");

    m_gui.AddButton(0, "credits", sf::Vector2f(342,320), "Credits");

    m_gui.AddButton(0, "exit", sf::Vector2f(355,365), "Close");

///////////////////////////////////////////////////////////////

    m_gui.AddLayer();

    m_gui.AddInput(1, "name", sf::Vector2f(315,160), "Levelname");

    m_gui.AddButton(1, "start", sf::Vector2f(350,220), "Start");

    m_gui.AddButton(1, "back", sf::Vector2f(355,360), "Back");

//////////////////////////////////////////////////////////////////////////////

    m_gui.AddLayer();

    m_gui.AddLabel(2, "0", sf::Vector2f(50,180), "Music by MafiaFLairBeatz");

    m_gui.AddLabel(2, "0", sf::Vector2f(50,220), "The great library SFML created by Laurent");

    m_gui.AddLabel(2, "0", sf::Vector2f(50,255), "Developed by Alexander Weinrauch");

    m_gui.AddButton(2, "back", sf::Vector2f(355,360), "Back");

///////////////////////////////////////////////////////////////////////

    m_gui.AddLayer();

    // Music on/off option
    m_gui.AddLabel(3, "music", sf::Vector2f(240,180), "Music: ");

    m_gui.AddList(3, "vol", sf::Vector2f(480,180), "on");
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

    // Music volume option
    m_gui.AddLabel(3, "music", sf::Vector2f(240,220), "Music volume: ");

    m_gui.AddList(3, "vol", sf::Vector2f(500,220), "50");
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
    m_gui.AddLabel(3, "anti", sf::Vector2f(240,260), "Antialiasing: ");

    m_gui.AddList(3, "antialiasing", sf::Vector2f(505,260), "0");
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
    m_gui.AddLabel(3, "tracertim", sf::Vector2f(240,300), "Time between \ntracer points: ");
    m_gui.GetLastLayer()->GetLastElement()->SetCharacterSize(14);

    m_gui.AddList(3, "tracertime", sf::Vector2f(480,300), "800");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("75");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("100");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("200");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("300");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("600");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("800");
    m_gui.GetLastLayer()->GetLastElement()->AddEntry("1000");


    switch(settings::GetTimeForTracer())
    {
        case 75:   index = 0; break;
        case 100:  index = 1; break;
        case 200:  index = 2; break;
        case 300:  index = 3; break;
        case 600:  index = 4; break;
        case 800:  index = 5; break;
        default :  index = 6; break;
    }
    m_gui.GetLastLayer()->GetLastElement()->SetActiveEntry(index);


    m_gui.AddButton(3, "back", sf::Vector2f(355,360), "Back");


    //////////////////////////////////////////////////////////////////

    m_gui.AddLayer(); // 4 Singleplayer layer

    m_gui.AddButton(4, "arcade layer", sf::Vector2f(346,170), "Arcade");


    m_gui.AddButton(4, "speedchallenge layer", sf::Vector2f(271,220), "Speed Challenge");

    m_gui.AddButton(4, "custommaps", sf::Vector2f(302,270), "Custom Maps");

    m_gui.AddButton(4, "back", sf::Vector2f(355,360), "Back");

    //////////////////////////////////////////////////////////////////

    m_gui.AddLayer();// 5 Arcade Mode


    m_gui.AddList(5, "lvl arcade", sf::Vector2f(335,170), "Select a level");

    for(int i = 0; i < settings::GetUnlockedLevel(); i++)
    {
        m_gui.GetLastLayer()->GetLastElement()->AddEntry(settings::GetLevelList().at(i));
    }

    if(settings::GetUnlockedLevel() > 1)
    {
        m_gui.GetLastLayer()->GetLastElement()->SetActiveEntry(settings::GetUnlockedLevel()-1);
    }
    m_gui.GetLastLayer()->GetLastElement()->SetObjectToCenter(m_window.getView().getSize().x);

    m_gui.AddButton(5, "startArcade", sf::Vector2f(353,220), "Start");


    m_gui.AddButton(5, "helparcade", sf::Vector2f(364,255), "Help");


    m_gui.AddButton(5, "back", sf::Vector2f(363,360), "Back");
       // std::cout << std::endl << 400 - m_gui.GetLastLayer()->GetLastElement()->GetTextObj().getLocalBounds().width / 2  << std::endl;
    ///////////////////////////////////////////////////////////////////////

    m_gui.AddLayer(); //6 Custommap mode

    m_mapManager.LoadMapList();

    float xPos = 200;

    if(m_mapManager.GetMapCount() == 0)
    {
        m_gui.AddList(6, "levellist", sf::Vector2f(xPos,140), "No levels found,\nplease update the list");
    }
    else
    {
        m_gui.AddList(6, "levellist", sf::Vector2f(xPos,140), "Select a level");

        for(unsigned int i = 0; i < m_mapManager.GetMapCount(); i++)
        {
            m_gui.GetLastLayer()->GetLastElement()->AddEntry(m_mapManager.GetMap(i)->name);
        }
    }

    m_gui.AddLabel(6, "author", sf::Vector2f(xPos,190), "Author: ");
    m_gui.AddLabel(6, "gamemmode", sf::Vector2f(xPos,220), "Gamemode: ");
    m_gui.AddLabel(6, "difficulty", sf::Vector2f(xPos,250), "Difficulty: ");
    m_gui.AddLabel(6, "speedxdisplay", sf::Vector2f(xPos, 290), "SpeedX: ");
    m_gui.AddLabel(6, "lengthdisplay", sf::Vector2f(xPos,310), "Length: ");

    m_gui.AddButton(6, "play", sf::Vector2f(xPos, 350), "Download");

    m_gui.AddButton(6, "updatelevellist", sf::Vector2f(xPos,390), "Update levellist");
    m_gui.AddButton(6, "back", sf::Vector2f(xPos,420), "Back");

}



void Menu::StartGame(std::string lvlname)
{
    std::fstream file;
    file.open(("levels/"+lvlname+"/settings.cfg").c_str(), std::ios::in);

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
            settings::SetGamemode(ARCADE);
        else if(gameMode == "Speed challenge")
            settings::SetGamemode(SPEED_CHALLENGE);
    }
}


