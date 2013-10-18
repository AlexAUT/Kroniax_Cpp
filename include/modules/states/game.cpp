#include "game.hpp"

#include <fstream>
#include <sstream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "../../messageBus/messageBus.hpp"

//Global Function
void initGui(aw::GuiController &gui);

namespace aw
{
	Game::Game(StateMachine &statemachine, MessageBus &messageBus):
		m_active(false),
		State(statemachine), 
		m_messageBus(messageBus),
		m_gameState(GameState::STOPPED)
	{
		m_messageBus.addReceiver(this);
		m_gui.loadFont("data/fonts/visitor1.ttf");

		initGui(m_gui); //Global Function
	}

	void Game::update(const sf::Time &frameTime)
	{
		m_active = true;

		if (m_music.getStatus() == sf::Music::Stopped)
		{
			m_music.play();
		}

		if (m_gameState == GameState::RUNNING)
		{
			//check for collision or finish
			auto result = m_collisionSystem.checkCollision(m_player);
			if (result == CollisionType::WALL)
			{
				m_gameState = GameState::CRASHED;
				m_gui.setActiveLayer(1);
				return;
			}
			else if (result == CollisionType::FINISH)
			{
				m_gameState = GameState::FINISHED;
				m_gui.setActiveLayer(2);
				return;
			}

			//Check for Scriptactions
			m_scriptManager.update(m_player, m_camera);

			//Update the position later, so the user see the crash
			//Update player position
			m_player.upate(frameTime);

			//update the camera position
			m_camera.update(m_player.getPosition());
		}
	}

	void Game::render(sf::RenderWindow &window)
	{
		//Set game view
		window.setView(m_camera.getGameView());
		//Render the map
		m_mapRenderer.render(window);
		//Draw script notification
		m_scriptManager.render(window);
		//Player
		m_player.render(window);
		//Set gui view
		window.setView(m_camera.getDefaultView());


		//Draw different screens
		if (m_gameState != GameState::RUNNING)
		{
			sf::RectangleShape overlay(sf::Vector2f(800, 450));
			overlay.setFillColor(sf::Color(0, 0, 0, 180));
			window.draw(overlay);
			m_gui.render(window);
		}
	}

	void Game::receiveMessage(const Message &msg)
	{
		//Loading the map
		if (msg.ID == std::hash<std::string>()("start game"))
		{
			//Set the name of the level
			m_levelName = *msg.getValue<std::string>(0);
			//Check if the Level is a tutorial
			if (m_levelName == "Tutorial2")
			{
				m_active = false;
				m_music.stop();
				changeActiveState("menu");
				Message msg(std::hash<std::string>()("Tutorial2"));
				m_messageBus.sendMessage(msg);
			}
			//Set the gametype
			if (*msg.getValue<std::string>(1) == "official arcade")
			{
				m_gameType = GameType::OFFICIAL_ARCADE;
			}
			//Load the level
			loadLevel();
		}
		//Set the volume of the music
		else if (msg.ID == std::hash<std::string>()("sound settings"))
		{
			m_music.setVolume(*msg.getValue<float>(1));
		}
		//Handling focus if lost = pause
		else if (msg.ID == std::hash<std::string>()("lost focus"))
		{
			//Pause the game if it is running..
			//In Start, Finish, Crash screen it's not needed to pause the game...
			if (m_gameState == GameState::RUNNING)
			{
				m_gameState = GameState::PAUSED;
				m_gui.setActiveLayer(3);
			}
		}
		//Starting the game
		else if (msg.ID == std::hash<std::string>()("event") && m_active)
		{
			sf::Event event = *msg.getValue<sf::Event>(0);

			//Give the event to gui
			m_gui.handleEvent(event);

			//Check for respawn etc.
			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Return)
				{
					if (m_gameState == GameState::STOPPED) 
					{
						m_gameState = GameState::RUNNING;
						resetToStart();
					}
					else if (m_gameState == GameState::CRASHED)
					{
						m_gameState = GameState::RUNNING;
						resetToLastCheckpoint();
					}
					else if (m_gameState == GameState::FINISHED)
					{
						if (m_gui.getSelectedElement()->getID() == "next")
						{
							//To inform the settings... unlock new/save progress + start next level
							sendInformationLevelFinished(true);
						}
						else if (m_gui.getSelectedElement()->getID() == "replay")
						{
							resetToStart();
							m_gameState = GameState::STOPPED;
							m_gui.setActiveLayer(0);
							//To inform the settings... unlock new/save progress
							sendInformationLevelFinished(false);
						}
						else if (m_gui.getSelectedElement()->getID() == "back")
						{
							changeActiveState("menu");
							m_music.stop();
							m_active = false;
							//To inform the settings... unlock new/save progress
							sendInformationLevelFinished(false);
						}
					}
					else if (m_gameState == GameState::PAUSED)
					{
						if (m_gui.getSelectedElement()->getID() == "resume")
						{
							m_gameState = GameState::RUNNING;
						}
						else if (m_gui.getSelectedElement()->getID() == "back")
						{
							changeActiveState("menu");
							m_music.stop();
							m_active = false;
						}
					}
				}
				else if (event.key.code == sf::Keyboard::BackSpace)
				{
					if (m_gameState == GameState::CRASHED)
					{
						m_gameState = GameState::RUNNING;
						resetToStart();
					}
				}
				else if (event.key.code == sf::Keyboard::Escape)
				{
					//What happens when pressing ESC depends on the current gamestate

					//Stopped or Crashed = back to menu
					if (m_gameState == GameState::STOPPED || m_gameState == GameState::CRASHED)
					{
						changeActiveState("menu");
						m_music.stop();
						m_active = false;
					}
					//Running = pause the game
					else if (m_gameState == GameState::RUNNING)
					{
						m_gameState = GameState::PAUSED;
						m_gui.setActiveLayer(3);
					}
					//Finish screen do nothing...
				}
			}
		}
	}


	////////// PRIVATE FUNCS ////////////

	void Game::initMusic()
	{
		if (m_gameType == GameType::OFFICIAL_ARCADE)
		{
			int levelnumber;
			std::size_t pos = m_levelName.find_last_of('l');
			std::string strlvlnumber = m_levelName.substr(pos + 1);
			std::stringstream sstr(strlvlnumber);
			sstr >> levelnumber;

			static int openMusic = -1;
			// 0 = level 1-5
			// 2 = level 6-10
			// 3 = level 11-15

			if (levelnumber < 6)
			{
				if (openMusic != 0)
					m_music.openFromFile("data/music/Galaxy - New Electro House Techno by MafiaFLairBeatz.ogg");

				openMusic = 0;
			}
			else if (levelnumber < 11)
			{
				if (openMusic != 1)
					m_music.openFromFile("data/music/MachinimaSound.com_-_After_Dark.ogg");

				openMusic = 1;
			}
		}
	}

	void Game::loadLevel()
	{
		//Reset all members first
		m_gameState = GameState::STOPPED;
		m_mapRenderer = MapRenderer();
		m_collisionSystem = CollisionSystem();
		m_player = Player();
		m_camera = Camera();
		m_scriptManager.deleteScripts();

		//Setup gamestate
		m_gameState = GameState::STOPPED;
		m_gui.setActiveLayer(0);

		//Setup the path for the level modules
		std::string path;
		if (m_gameType == GameType::OFFICIAL_ARCADE || m_gameType == GameType::OFFICIAL_TIMECHALLENGE)
		{
			path = "data/levels/official/" + m_levelName + ".cfg";
		}
		else
		{
			path = "data/levels/custom/" + m_levelName + ".cfg";
		}

		//Load all modules
		//Load the optical part of the map
		m_mapRenderer.load(path);
		//Load the collision map (Walls and Finish)
		m_collisionSystem.loadMap(path);
		//Load player information (speed, gravity, spawn)
		m_player.loadInformation(path);
		//Call player update with 0 frametime to prevent a instant death.
		m_player.upate(sf::Time());
		//Update Camera position
		m_camera.update(m_player.getPosition());
		//Load all the scripts
		m_scriptManager.load(path);
		//Open the right Music
		initMusic();
	}


	void Game::resetToLastCheckpoint()
	{
		auto ptr = m_scriptManager.getLastCheckPoint();
		if (ptr)
		{
			//Triggered checkpoint found -> reset to it
			m_player = ptr->savedPlayer;
			m_camera = ptr->savedCamera;
			//Reset scripts except checkpoint = parameter(false)
			m_scriptManager.resetScriptStates(false);
		}
		else
		{
			//No checkpoint triggered -> reset to start
			resetToStart();
		}
	}

	void Game::resetToStart()
	{
		//Reset player
		m_player.resetToStartSettings();

		//Reset Camera
		m_camera = Camera();

		//Reset scripts
		m_scriptManager.resetScriptStates();;
	}

	void Game::sendInformationLevelFinished(bool startNextLevel)
	{
		Message msg;
		msg.ID = std::hash<std::string>()("level complete");
		msg.push_back(m_levelName);
		msg.push_back(startNextLevel);
		m_messageBus.sendMessage(msg);
	}
}



//GLOBAL FUNCTION

void initGui(aw::GuiController &gui)
{
	//Stopped Layer
	//How to start the game
	gui.addLayer();
	//The gui needs at least one selectable object for each layer ->placeholder
	gui.addButton(0, "placeholder", sf::Vector2f(-100, -100), "");
	////////////////////////////////////////////////////////////////
	gui.addButton(0, "msg", sf::Vector2f(227, 135), "Enter: Start the Game");
	gui.getElement(0, 1)->setCharacterSize(30);
	gui.getElement(0, 1)->setSelectable(false);
	gui.addButton(0, "msg2", sf::Vector2f(275, 250), "Escape: Return to menu");
	gui.getElement(0, 2)->setCharacterSize(20);
	gui.getElement(0, 2)->setSelectable(false);
	gui.setActiveLayer(0);
	//Layer after crashing into a wall
	gui.addLayer();
	//The gui needs at least one selectable object for each layer ->placeholder
	gui.addButton(1, "placeholder", sf::Vector2f(-100, -100), "");
	////////////////////////////////////////////////////////////////
	gui.addButton(1, "msg", sf::Vector2f(75, 135), "Enter: Start the Game from checkpoint");
	gui.getElement(1, 1)->setCharacterSize(30);
	gui.getElement(1, 1)->setSelectable(false);
	gui.getElement(1, 1)->setSelected(false);
	gui.addButton(1, "msg2", sf::Vector2f(150, 250), "Back space: Start the game from Start");
	gui.getElement(1, 2)->setCharacterSize(20);
	gui.getElement(1, 2)->setSelectable(false);

	//Layer finish official_Arcade
	gui.addLayer();
	gui.addButton(2, "next", sf::Vector2f(250, 180), "Start next level");
	gui.addButton(2, "replay", sf::Vector2f(250, 210), "Replay this level");
	gui.addButton(2, "back", sf::Vector2f(250, 240), "Return to menu");
	gui.addLabel(2, "headline", sf::Vector2f(40, 80), "Great you have completed this level!");
	gui.getElement(2, 3)->setCharacterSize(35);
	gui.addLabel(2, "question", sf::Vector2f(80, 180), "What to do: ");

	//Pause Layer
	gui.addLayer();
	gui.addButton(3, "resume", sf::Vector2f(250, 200), "Continue");
	gui.addButton(3, "back", sf::Vector2f(250, 250), "Back to menu");
	gui.addLabel(3, "", sf::Vector2f(250, 100), "Game is paused");
	gui.getElement(3, 2)->setCharacterSize(35);
	

}