#include "menu.hpp"

#include "../../messageBus/messageBus.hpp"
#include "../../utilities/hash.hpp"

#include <fstream>
#include <sstream>

#include <random>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

//Global gui init function, to make the code easier to read
void initMainLayer(aw::GuiController &gui);
void initArcadeLayer(aw::GuiController &gui);
void initTutorialLayers(aw::GuiController &gui);
void initOptionsLayer(aw::GuiController &gui);
void initCreditsLayer(aw::GuiController &gui);
void initMultiplayerLayers(aw::GuiController &gui);
void initMultiplayerHelpLayer(aw::GuiController &gui);

namespace aw
{
	Menu::Menu(StateMachine &stateMachine, MessageBus &messageBus):		
		State(stateMachine),
        m_active(false),
		m_messageBus(messageBus)
	{
		m_messageBus.addReceiver(this);

		initGui();

		loadRandomLevel();

		m_overlay.setPosition(sf::Vector2f(0.f, 0.f));
		m_overlay.setSize(sf::Vector2f(800.f, 450.f));
		m_overlay.setFillColor(sf::Color(0, 0, 0, 180));

		m_texLogo.loadFromFile("data/images/kroniax.png");
		m_logo.setTexture(m_texLogo);
		m_logo.setScale(0.6f, 0.6f);
		m_logo.setPosition(215.f, -25.f);

		resetView();

		m_music.openFromFile("data/music/Power Fight - Electro Techno Beat.ogg");
	}

	void Menu::initGui()
	{
		m_gui.loadFont("data/fonts/visitor1.ttf");

		//Layer 0
		initMainLayer(m_gui);
		//Layer1
		initArcadeLayer(m_gui);
		//Layer2,3,4
		initTutorialLayers(m_gui);
		//Layer5
		initOptionsLayer(m_gui);
		//Layer6
		initCreditsLayer(m_gui);
		//Layer7,8,9
		initMultiplayerLayers(m_gui);
		//Layer10
		initMultiplayerHelpLayer(m_gui);
	}

	void Menu::update(const sf::Time &frameTime)
	{
		m_active = true;
		//Update view, so the map scroll in the background
		if (m_music.getStatus() != sf::Music::Status::Playing)
			m_music.play();

		m_view.move(sf::Vector2f(400.f * frameTime.asSeconds(), 0.f));

		if (m_mapRenderer.isOnTheEnd(static_cast<std::size_t>(m_view.getCenter().x)))
		{
			loadRandomLevel();
			m_view.setCenter(sf::Vector2f(400, 225));
		}

		//Update the levelinformation 
		updateLevelInformation();
	}

	void Menu::render(sf::RenderWindow &window)
	{
		sf::View defaultView = window.getView();
		window.setView(m_view);
		m_mapRenderer.render(window);
		window.setView(defaultView);
		//Grey overlay for the background
	
		window.draw(m_overlay);
		window.draw(m_logo);
		m_gui.render(window);
	}


	void Menu::receiveMessage(const Message &msg)
	{
		if (msg.ID == aw::hash("sound settings"))
		{
			m_music.setVolume(*msg.getValue<float>(0));
			//Update the gui to disply the right values
			//check if the volume is in range
			unsigned int index = static_cast<unsigned int>(*msg.getValue<float>(0));
			if (index < 100)
			{
				m_gui.getElement(5, 0)->setActiveEntry(index);
			}
			//check if the volume is in range
			index = static_cast<unsigned int>(*msg.getValue<float>(1));
			if (index < 100)
			{
				m_gui.getElement(5, 1)->setActiveEntry(index);
			}
		}
		else if (msg.ID == aw::hash("window settings"))
		{
			//Update the gui to display correct information
			//Fullscreen
			if (*msg.getValue<bool>(3))
			{
				m_gui.getElement(5, 3)->setActiveEntry(1);
			}
			else
			{
				m_gui.getElement(5, 3)->setActiveEntry(0);
			}
			//Antialiasinglevel
			switch (*msg.getValue<unsigned int>(4))
			{
			case 0: m_gui.getElement(5, 2)->setActiveEntry(0); break;
			case 2: m_gui.getElement(5, 2)->setActiveEntry(1); break;
			case 4: m_gui.getElement(5, 2)->setActiveEntry(2); break;
			case 8: m_gui.getElement(5, 2)->setActiveEntry(3); break;
			case 16: m_gui.getElement(5, 2)->setActiveEntry(4); break;
			default: m_gui.getElement(5, 2)->setActiveEntry(0); break;
			}
		}
		else if (msg.ID == aw::hash("event") && m_active)
		{
			sf::Event event = *msg.getValue<sf::Event>(0);

			m_gui.handleEvent(event);

			if (event.type == sf::Event::KeyReleased && (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Return))
			{
				buttonAction();
			}
		}
		else if (msg.ID == aw::hash("start game"))
		{
			m_music.stop();
			m_active = false;
			changeActiveState("game");
		}
		else if (msg.ID == aw::hash("unlocked levels"))
		{
			m_unlockedLevels = *msg.getValue<unsigned int>(0);
		}
		else if (msg.ID == aw::hash("arcade levellist"))
		{
			m_gui.getElement(1, 0)->clearEntries();
			//Fill the level selection list, with the unlocked levels
			for (std::size_t i = 0; i <= m_unlockedLevels; i++)
			{
				m_gui.getElement(1, 0)->addEntry(*msg.getValue<std::string>(i));
			}
		}
		else if (msg.ID == aw::hash("Tutorial2"))
		{
			m_gui.setActiveLayer(3);
		}
		////////////////////// MULTIPLAYER ACTIONS ////////////////////////////////
		else if (msg.ID == aw::hash("result connecting"))
		{
			if (*msg.getValue<bool>(0))
			{
				//Map Selection
				m_gui.setActiveLayer(9);

				Message msg(aw::hash("request game list"));
				m_messageBus.sendMessage(msg);
			}
			else
			{
				//Connection failed layer
				m_gui.setActiveLayer(8);
			}
		}
		else if (msg.ID == aw::hash("game list"))
		{
			m_gui.getElement(9, 0)->clearEntries();

			for (std::size_t i = 0;; ++i)
			{
				std::string *ptr = msg.getValue<std::string>(i);

				if (ptr)
				{
					m_gui.getElement(9, 0)->addEntry(*ptr);
				}
				else
				{
					break;
				}
			}
		}
	}

	void Menu::resetView()
	{
		m_view = sf::View(sf::FloatRect(0, 0, 800, 450));
	}

	void Menu::loadRandomLevel()
	{
		//Random with level will be load from the brackground(old Levels)
		// Seed with a real random value, if available
		std::random_device rd;

		// Choose a random range
		std::default_random_engine e1(rd());
		std::uniform_int_distribution<int> uniform_dist(1, 2);
		int levelNumber = uniform_dist(e1);
		std::stringstream sstr;
		sstr << levelNumber;
		std::string path = "data/levels/old levels/Level" + sstr.str() + "_old.cfg";
		m_mapRenderer.load(path);
	}


	void Menu::updateLevelInformation()
	{
		//Check which layer is active
		if (m_gui.getActiveLayerInt() == 1)
		{
			//Arcade layer
			std::string name = m_gui.getElement(1, 0)->getText();
			//Check if update is needed
			if (m_levelInformation.name != name)
			{
				std::fstream file(("data/levels/official/" + name + ".cfg").c_str());

				if (file.good())
				{
					std::string line;
					while (std::getline(file, line))
					{
						if (line == "[Name]")
						{
							std::getline(file, line);
							m_levelInformation.name = line;
						}
						else if (line == "[Author]")
						{
							std::getline(file, line);
							m_levelInformation.author = line;
						}
						else if (line == "[Difficulty]")
						{
							std::getline(file, line);
							m_levelInformation.difficulty = line;
						}
						else if (line == "[Length]")
						{
							std::getline(file, line);
							m_levelInformation.lentgh = line;
							break;
						}
					}
				}
				else
				{
					//Failed to load
					if (name.substr(0, 8) == "Tutorial")
					{
						//Set name and author
						m_levelInformation.name = name;
						m_levelInformation.author = "AlexAUT";
						//A tutorial has no difficulty or length
						m_levelInformation.difficulty = "";
						m_levelInformation.lentgh = "";
					}
					else if (name != "Select a level")
					{
						m_levelInformation.name = "Failed to load";
						m_levelInformation.author = "Failed to load";
						m_levelInformation.difficulty = "Failed to load";
						m_levelInformation.lentgh = "Failed to load";
					}
				}

				//Update the display
				m_gui.getElement(1, 1)->setText("Name: " + m_levelInformation.name);
				m_gui.getElement(1, 2)->setText("Difficulty: " + m_levelInformation.difficulty);
				m_gui.getElement(1, 3)->setText("Length: " + m_levelInformation.lentgh);
				m_gui.getElement(1, 4)->setText("Author: " + m_levelInformation.author);
			}
		}
	}


	//////////////BUTTON FUNCTIONS//////////////////////

	void Menu::buttonAction()
	{
		switch (m_gui.getActiveLayerInt())
		{
		case 0: mainLayer(); break;
		case 1: arcadeLayer(); break;
		case 2: tutorial1Layer(); break;
		case 3: tutorial2Layer(); break;
		case 5: optionsLayer(); break;
		case 6: creditsLayer(); break;
		case 7: setUpConnectionLayer(); break;
		case 8: connectionFailedLayer(); break;
		case 9: mapSelectionLayer(); break;
		case 10: helpMultiplayerLayer(); break;
		default: break;
		}
	}

	void Menu::mainLayer()
	{
		if (m_gui.getSelectedElement()->getID() == "arcade")
		{
			m_gui.setActiveLayer(1);
		}
		else if (m_gui.getSelectedElement()->getID() == "time challenge")
		{

		}
		else if (m_gui.getSelectedElement()->getID() == "multiplayer")
		{
			m_gui.setActiveLayer(7);
		}
		else if (m_gui.getSelectedElement()->getID() == "options")
		{
			m_gui.setActiveLayer(5);
		}
		else if (m_gui.getSelectedElement()->getID() == "credits")
		{
			m_gui.setActiveLayer(6);
		}
		else if (m_gui.getSelectedElement()->getID() == "exit")
		{
			Message msg;
			msg.ID = aw::hash("close game");
			m_messageBus.sendMessage(msg);
		}
	}

	void Menu::arcadeLayer()
	{
		if (m_gui.getSelectedElement()->getID() == "select level" && m_gui.getSelectedElement()->getText() != "Select a level")
		{
			//Handle the different Tutorials... (show a unique gui layer...
			if (m_levelInformation.name == "Tutorial1")
			{
				m_gui.setActiveLayer(2);
			}
			else if (m_levelInformation.name == "Tutorial2")
			{
				m_gui.setActiveLayer(3);
			}
			else
			{
				//For all levels except tutorials
				//Send message... So the game will start...
				Message msg;
				msg.ID = aw::hash("start game");
				msg.push_back(m_levelInformation.name);
				msg.push_back(static_cast<std::string>("official arcade"));
				m_messageBus.sendMessage(msg);
				changeActiveState("game");
				m_music.stop();
				m_active = false;
			}
		}
		else if (m_gui.getSelectedElement()->getID() == "back arcade")
		{
			m_gui.setActiveLayer(0);
		}
	}

	void Menu::tutorial1Layer()
	{
		if (m_gui.getSelectedElement()->getID() == "start level1")
		{
			//start level1
			//Send message... So the game will start...
			Message msg;
			msg.ID = aw::hash("start game");
			std::string name = "Level1";
			msg.push_back(name);
			msg.push_back(static_cast<std::string>("official arcade"));
			m_messageBus.sendMessage(msg);
			changeActiveState("game");
			m_music.stop();
			m_active = false;
			m_gui.setActiveLayer(1);
		}
		else if (m_gui.getSelectedElement()->getID() == "back")
		{
			m_gui.setActiveLayer(1);
		}
	}
	
	void Menu::tutorial2Layer()
	{
		if (m_gui.getSelectedElement()->getID() == "start level4")
		{
			//start level6
			//Send message... So the game will start...
			Message msg;
			msg.ID = aw::hash("start game");
			std::string name = "Level4";
			msg.push_back(name);
			msg.push_back(static_cast<std::string>("official arcade"));
			m_messageBus.sendMessage(msg);
			changeActiveState("game");
			m_music.stop();
			m_active = false;
			m_gui.setActiveLayer(1);
		}
		else if (m_gui.getSelectedElement()->getID() == "back")
		{
			m_gui.setActiveLayer(1);
		}
	}

	void Menu::optionsLayer()
	{
		if (m_gui.getSelectedElement()->getID() == "reset graphics")
		{
			Message msg(aw::hash("reset resolution"));
			m_messageBus.sendMessage(msg);
		}
		else if (m_gui.getSelectedElement()->getID() == "apply")
		{
			Message msgSound(aw::hash("new sound settings"));
			//Menu and game volume
			msgSound.push_back(m_gui.getElement(5, 0)->getText());
			msgSound.push_back(m_gui.getElement(5, 1)->getText());
			m_messageBus.sendMessage(msgSound);

			Message msgWindow(aw::hash("new window settings"));
			//Antialiasinglevel and fullscreen
			msgWindow.push_back(m_gui.getElement(5, 2)->getText());
			msgWindow.push_back(m_gui.getElement(5, 3)->getText());
			m_messageBus.sendMessage(msgWindow);

			m_gui.setActiveLayer(0);
		}
		else if (m_gui.getSelectedElement()->getID() == "back")
		{
			m_gui.setActiveLayer(0);
		}
	}

	void Menu::creditsLayer()
	{
		if (m_gui.getSelectedElement()->getID() == "back")
		{
			m_gui.setActiveLayer(0);
		}
	}

	void Menu::setUpConnectionLayer()
	{
		if (m_gui.getSelectedElement()->getID() == "submit")
		{
			Message msg;
			msg.ID = aw::hash("connect");
			//Pushback the name of the player
			msg.push_back(m_gui.getElement(7, 0)->getText());
			
			m_messageBus.sendMessage(msg);
		}
		else if (m_gui.getSelectedElement()->getID() == "back")
		{
			m_gui.setActiveLayer(0);
		}
	}

	void Menu::connectionFailedLayer()
	{
		if (m_gui.getSelectedElement()->getID() == "back")
		{
			m_gui.setActiveLayer(7);
		}
	}

	void Menu::mapSelectionLayer()
	{
		if (m_gui.getSelectedElement()->getID() == "select server")
		{
			//Store the information, the MultiplayerHelp layer will send the information
			m_messageMutliplayerStart.ID = aw::hash("join server");
			m_messageMutliplayerStart.clear();
			m_messageMutliplayerStart.push_back(m_gui.getSelectedElement()->getText());
			m_gui.setActiveLayer(10);
		}
		else if (m_gui.getSelectedElement()->getID() == "back")
		{
			m_gui.setActiveLayer(0);
		}
	}

	void Menu::helpMultiplayerLayer()
	{
		if (m_gui.getSelectedElement()->getID() == "start")
		{
			m_messageBus.sendMessage(m_messageMutliplayerStart);
			m_gui.setActiveLayer(9);
		}
	}
}


void initMainLayer(aw::GuiController &gui)
{
	//Mainlayer = 0
	gui.addLayer();
	gui.setActiveLayer(0);
	gui.addButton(0, "arcade", sf::Vector2f(275, 125), "Arcade");
	gui.addButton(0, "time challenge", sf::Vector2f(275, 165), "Time Challenge");
	gui.addButton(0, "multiplayer", sf::Vector2f(275, 205), "Multiplayer");
	gui.addButton(0, "options", sf::Vector2f(275, 275), "Options");
	gui.addButton(0, "credits", sf::Vector2f(275, 305), "Credits");
	gui.addButton(0, "exit", sf::Vector2f(275, 375), "Close");
}

void initArcadeLayer(aw::GuiController &gui)
{
	//Arcadelayer = 1
	gui.addLayer();
	gui.addList(1, "select level", sf::Vector2f(275, 165), "Select a level");
	gui.addLabel(1, "name", sf::Vector2f(275, 220), "Name: ");
	gui.addLabel(1, "difficulty", sf::Vector2f(275, 255), "Difficulty: ");
	gui.addLabel(1, "lentgh", sf::Vector2f(275, 290), "Length: ");
	gui.addLabel(1, "author", sf::Vector2f(275, 325), "Author: ");
	gui.addButton(1, "back arcade", sf::Vector2f(300, 385), "Back");
}

void initTutorialLayers(aw::GuiController &gui)
{
	//Tutorial1 = layer(2)
	gui.addLayer();
	gui.addButton(2, "start level1", sf::Vector2f(300, 345), "Start Level1");
	gui.addButton(2, "back", sf::Vector2f(297, 375), "Back to menu");
	gui.addLabel(2, "text", sf::Vector2f(50, 110), "The target of the game is to steer the ship through\nthe" 
		"level without touching the walls.\n\n\nYou can steer up your ship by pressing the spacebar.\n\n\nOrange" 
		"horizontal lines are checkpoints\nWhen you hit a wall you will respawn there");


	//Will be implemented when needed...
	//Tutorial2 = layer(3)
	gui.addLayer();
	gui.addButton(3, "start level6", sf::Vector2f(300, 345), "Start Level4");
	gui.addButton(3, "back", sf::Vector2f(297, 375), "Back to menu");
	gui.addLabel(3, "text", sf::Vector2f(50, 120), "Some more Information:\n\n\nGreen horizontal lines will" 
		"change your speed!\n\nYellow horizontal lines will change your gravity!\n\nBlue lines will affect your view!");

	//Tutorial3 = layer(4)
	gui.addLayer();

}

void initOptionsLayer(aw::GuiController &gui)
{
	//Layer 5
	gui.addLayer();
	//Add list for changing the menu volume + fill the entries 0-100
	gui.addList(5, "menu volume", sf::Vector2f(500, 125), "5");
	for (int i = 0; i < 101; ++i)
	{
		std::stringstream sstr;
		sstr << i;
		gui.getElement(5, 0)->addEntry(sstr.str());
	}
	//Add list for changing the game volume + fill the entries 0-100
	gui.addList(5, "game volume", sf::Vector2f(500, 155), "5");
	for (int i = 0; i < 101; ++i)
	{
		std::stringstream sstr;
		sstr << i;
		gui.getElement(5, 1)->addEntry(sstr.str());
	}
	//Add list for changing the antialiasinglevel + fill the entries 0,2,4,8,16
	gui.addList(5, "antialiasinglevel", sf::Vector2f(500, 200), "0");
	gui.getElement(5, 2)->addEntry("0");
	gui.getElement(5, 2)->addEntry("2");
	gui.getElement(5, 2)->addEntry("4");
	gui.getElement(5, 2)->addEntry("8");
	gui.getElement(5, 2)->addEntry("16");
	//Add list for changing fullscreen mode, entries = off and on
	gui.addList(5, "fullscreen", sf::Vector2f(500, 230), "off");
	gui.getElement(5, 3)->addEntry("off");
	gui.getElement(5, 3)->addEntry("on");

	//Add labels (description of the lists
	gui.addLabel(5, "ssv", sf::Vector2f(250, 125), "Menu volume:");
	gui.addLabel(5, "sgv", sf::Vector2f(250, 155), "Ingame volume:");
	gui.addLabel(5, "ant", sf::Vector2f(250, 200), "Antialiasinglevel:");
	gui.addLabel(5, "rs", sf::Vector2f(250, 230), "Fullscreen:");
	//Will reset the resolution in windowmode
	gui.addButton(5, "reset graphics", sf::Vector2f(250, 275), "Reset default resolution");

	//Save+back and only back...
	gui.addButton(5, "apply", sf::Vector2f(275, 350), "Apply");
	gui.addButton(5, "back", sf::Vector2f(275, 385), "Back without saving");
}

void initCreditsLayer(aw::GuiController &gui)
{
	//Layer 6
	gui.addLayer();
	gui.addButton(6, "back", sf::Vector2f(175, 385), "Back");

	gui.addLabel(6, "credit", sf::Vector2f(50, 145), "Developed by Alexander Weinrauch (AlexAUT).");
	gui.addLabel(6, "credit", sf::Vector2f(50, 195), "Special Thanks to:");
	gui.addLabel(6, "credit", sf::Vector2f(150, 235), "Laurent for SFML!");
	gui.addLabel(6, "credit", sf::Vector2f(150, 265), "fallahn for sfChat (Chat system)");
	gui.addLabel(6, "credit", sf::Vector2f(150, 295), "MafiaFLairBeatz for the music!");
	gui.addLabel(6, "credit", sf::Vector2f(150, 325), "Machinimasound for the music!");
}

void initMultiplayerLayers(aw::GuiController &gui)
{
	//Layer 7 // set up connection layer
	gui.addLayer();
	gui.addInput(7, "name", sf::Vector2f(345, 125), "Player");
	gui.addLabel(7, "", sf::Vector2f(250, 125), "Name:");
	gui.addButton(7, "submit", sf::Vector2f(325, 200), "Submit");
	gui.addButton(7, "back", sf::Vector2f(325, 240), "Back");

	//Layer 8 //Connection failed layer
	gui.addLayer();
	gui.addButton(8, "back", sf::Vector2f(325, 240), "Back");
	gui.addLabel(8, "", sf::Vector2f(250, 125), "Couldn't connect to the server!");

	//Layer 9 //MapSelection layer
	gui.addLayer();
	gui.addList(9, "select server", sf::Vector2f(275, 165), "Select a server");
	gui.addLabel(9, "map", sf::Vector2f(275, 220), "Current map: ");
	gui.addLabel(9, "player", sf::Vector2f(275, 255), "Player: ");
	//gui.addLabel(9, "lentgh", sf::Vector2f(275, 290), "Length: ");
	//gui.addLabel(9, "author", sf::Vector2f(275, 325), "Author: ");
	gui.addButton(9, "back", sf::Vector2f(300, 385), "Back");
}

void initMultiplayerHelpLayer(aw::GuiController &gui)
{
	//Layer10
	gui.addLayer();
	gui.addButton(10, "start", sf::Vector2f(360, 350), "Start");

	gui.addLabel(10, "", sf::Vector2f(80, 125), "Try to get the fastest time possible!\n\n\n"
		"Use the arrow keys to control your speed\n\n"
		"Hold tab to display the scoreboard\n\n"
		"Press T to open the chat and Escape to close it");

}
