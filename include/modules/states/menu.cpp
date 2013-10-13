#include "menu.hpp"

#include "../../messageBus/messageBus.hpp"

#include <fstream>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

//Global gui init function, to make the code easier to read
void initMainLayer(aw::GuiController &gui);
void initArcadeLayer(aw::GuiController &gui);
void initTutorialLayers(aw::GuiController &gui);

namespace aw
{
	Menu::Menu(StateMachine &stateMachine, MessageBus &messageBus):
		m_active(false),
		State(stateMachine),
		m_messageBus(messageBus)
	{
		m_messageBus.addReceiver(this);

		initGui();

		m_mapRenderer.load("data/levels/official/Level1.cfg");

		m_overlay.setPosition(sf::Vector2f(0.f, 0.f));
		m_overlay.setSize(sf::Vector2f(800.f, 450.f));
		m_overlay.setFillColor(sf::Color(0, 0, 0, 180));

		m_texLogo.loadFromFile("data/images/logo2.png");
		m_logo.setTexture(m_texLogo);
		m_logo.setScale(0.5f, 0.5f);
		m_logo.setPosition(255.f, -40.f);

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
		//Layer2
		initTutorialLayers(m_gui);
	}

	void Menu::update(const sf::Time &frameTime)
	{
		m_active = true;
		//Update view, so the map scroll in the background
		if (m_music.getStatus() != sf::Music::Status::Playing)
			m_music.play();

		m_view.move(sf::Vector2f(200.f * frameTime.asSeconds(), 0.f));

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
		if (msg.ID == std::hash<std::string>()("sound settings"))
		{
			m_music.setVolume(*msg.getValue<float>(0));
		}
		else if (msg.ID == std::hash<std::string>()("event") && m_active)
		{
			sf::Event event = *msg.getValue<sf::Event>(0);

			m_gui.handleEvent(event);

			if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Return))
			{
				buttonAction();
			}
		}
		else if (msg.ID == std::hash<std::string>()("unlocked levels"))
		{
			m_unlockedLevels = *msg.getValue<unsigned int>(0);
		}
		else if (msg.ID == std::hash<std::string>()("arcade levellist"))
		{
			m_gui.getElement(1, 0)->clearEntries();
			//Fill the level selection list, with the unlocked levels
			for (std::size_t i = 0; i <= m_unlockedLevels; i++)
			{
				m_gui.getElement(1, 0)->addEntry(*msg.getValue<std::string>(i));
			}
		}
	}

	void Menu::resetView()
	{
		m_view = sf::View(sf::FloatRect(0, 0, 800, 450));
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
					if (name != "Select a level")
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

		}
		else if (m_gui.getSelectedElement()->getID() == "options")
		{

		}
		else if (m_gui.getSelectedElement()->getID() == "credits")
		{

		}
		else if (m_gui.getSelectedElement()->getID() == "exit")
		{
			Message msg;
			msg.ID = std::hash<std::string>()("close game");
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
			else
			{
				//For all levels except tutorials
				//Send message... So the game will start...
				Message msg;
				msg.ID = std::hash<std::string>()("start game");
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
			msg.ID = std::hash<std::string>()("start game");
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
	gui.addLabel(2, "text", sf::Vector2f(50, 110), "The target of the game is to steer the ship through\nthe level without touching the walls.\n\n\nYou can steer up your ship by pressing the spacebar.\n\n\nOrange vertical Lines are checkpoints\nWhen you hit a wall you will respawn there");

}