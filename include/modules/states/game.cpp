#include "game.hpp"

#include <fstream>
#include <sstream>

#include <iostream>

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
		m_gameState(GameState::STOPPED),
		m_openMusic(-1)
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
		//	m_music.play();
		}
		
		//Update the online countdown
		m_countDownNextAction.update(frameTime.asSeconds());

		//Check the onlinestate to prevent the game from wrong starts
		//In singleplayer onlinemode is always Running
		if (m_gameState == GameState::RUNNING && m_onlineState == OnlineState::RUNNING)
		{
			//check for collision or finish
			auto result = m_collisionSystem.checkCollision(m_players[0]);
			if (result == CollisionType::WALL)
			{
				m_gameState = GameState::CRASHED;
				m_gui.setActiveLayer(1);
				return;
			}
			else if (result == CollisionType::FINISH)
			{
				m_gameState = GameState::FINISHED;
				if (m_gameType == GameType::OFFICIAL_ARCADE)
				{
					m_gui.setActiveLayer(2);
				}
				else if (m_gameType == GameType::ONLINE_TIME_CHALLENGE)
				{
					m_gameState = GameState::STOPPED;
					//Send information to the server
					Message msg;
					msg.ID = std::hash<std::string>()("new time");
					msg.push_back(m_gameTimer.getTime());
					m_messageBus.sendMessage(msg);

					resetToStart();
					m_gui.setActiveLayer(0);
				}
				

				return;
			}

			//Check for Scriptactions
			m_scriptManager.update(m_players[0], m_camera);

			//Update the position later, so the user see the crash
			//Update player position
			for (auto &it : m_players)
			{
				it.update(frameTime);
			}

			//update the camera position
			m_camera.update(m_players[0].getPosition());

			//update the game timer only when running
			m_gameTimer.update(frameTime.asSeconds());
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
		for (auto &it : m_players)
		{
			it.render(window);
		}
		//Set gui view
		window.setView(m_camera.getDefaultView());
		//Draw gameTimer
		m_gameTimer.render(window);
		//Draw different screens 
		if (m_gameState != GameState::RUNNING && m_onlineState == OnlineState::RUNNING)
		{
			sf::RectangleShape overlay(sf::Vector2f(800, 450));
			overlay.setFillColor(sf::Color(0, 0, 0, 180));
			window.draw(overlay);
			m_gui.render(window);
		}

		//Draw online scrren depending on the onlinestate
		if (m_onlineState == OnlineState::LOADING)
		{
			m_timeTable.render(window);
			m_countDownNextAction.render(window, "until the game will start");
		}
		else if (m_onlineState == OnlineState::FINISHED)
		{
			m_timeTable.render(window);
			m_countDownNextAction.render(window, "until the next level");
		}
		else if (m_onlineState == OnlineState::RUNNING)
		{
			m_countDownNextAction.render(window, "for this map!");
		}
		//Draw timetable when holding tab (only in online mode)
		if (m_gameType == GameType::ONLINE_TIME_CHALLENGE && m_onlineState == OnlineState::RUNNING)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
			{
				m_timeTable.render(window);
			}
		}
	}

	void Game::receiveMessage(const Message &msg)
	{
		//Loading the map
		if (msg.ID == std::hash<std::string>()("start game"))
		{
			//Set the name of the level
			m_levelName = *msg.getValue<std::string>(0);
			std::cout << "Mapname: " << m_levelName << std::endl;
			//Check if the Level is a tutorial
			if (m_levelName == "Tutorial2")
			{
				m_active = false;
				m_music.stop();
				changeActiveState("menu");
				quitOnlineGame();
				Message msg(std::hash<std::string>()("Tutorial2"));
				m_messageBus.sendMessage(msg);
			}
			//Set the gametype
			if (*msg.getValue<std::string>(1) == "official arcade")
			{
				m_gameType = GameType::OFFICIAL_ARCADE;
				m_onlineState = OnlineState::RUNNING;
			}
			else if (*msg.getValue<std::string>(1) == "online time challenge")
			{
				m_gameType = GameType::ONLINE_TIME_CHALLENGE;
				//OnlineState
				m_onlineState = OnlineState(*msg.getValue<int>(2));
				m_countDownNextAction.setTime(*msg.getValue<float>(3));
			}
			//Set up the player vector
			//Push_back main player [0] is ALWAYS the you^^
			//[0] is always the on this device
			m_players.clear();
			m_players.push_back(Player());

			//Load the level
			loadLevel();

			if (m_gameType == GameType::ONLINE_TIME_CHALLENGE)
			{
				//Set up all online players
				for (std::size_t i = 0; i < m_players.size(); ++i)
				{
					m_players[i] = m_players[0];
				}
				//load players and times
				for (std::size_t i = 4;; i += 2)
				{
					std::string *name = msg.getValue<std::string>(i);
					float *time = msg.getValue<float>(i + 1);

					if (name && time)
					{
						m_timeTable.addPlayer(*name, *time);
						m_players.push_back(Player());
					}
					else
					{
						break;
					}
				}
			}

		}
		//New player connected to the game = add player to m_players and m_timeTable
		else if (msg.ID == std::hash<std::string>()("new player"))
		{
			m_players.push_back(Player());
			m_timeTable.addPlayer(*msg.getValue<std::string>(0));
		}
		//Remove a player from m_players and timetable (same index)
		else if (msg.ID == std::hash<std::string>()("remove player"))
		{
			std::size_t index = m_timeTable.getPlayerIndex(*msg.getValue<std::string>(0));
			m_timeTable.removePlayer(index);
			m_players.erase(m_players.begin() + index);
		}
		//After onlinemode finish -> this should load a new map and reset map/player's time
		else if (msg.ID == std::hash<std::string>()("new map"))
		{
			m_levelName = *msg.getValue<std::string>(0);
			loadLevel();

			//Reset online players
			for (std::size_t i = 0; i < m_players.size(); ++i)
			{
				m_players[i] = m_players[0];
			}
			//Reset timeTable times
			m_timeTable.resetTimes();
		}
		else if (msg.ID == std::hash<std::string>()("onlinemode loading"))
		{
			m_onlineState = OnlineState::LOADING;
			m_countDownNextAction.setTime(*msg.getValue<float>(0));
		}
		else if (msg.ID == std::hash<std::string>()("onlinemode running"))
		{
			m_onlineState = OnlineState::RUNNING;
			m_countDownNextAction.setTime(*msg.getValue<float>(0));
		}
		else if (msg.ID == std::hash<std::string>()("onlinemode finish"))
		{
			m_onlineState = OnlineState::FINISHED;
			m_countDownNextAction.setTime(*msg.getValue<float>(0));
		}
		//A player got a new besttime, add to timetable
		else if (msg.ID == std::hash<std::string>()("new best time"))
		{
			std::cout << "new time! : " << *msg.getValue<std::string>(0) << " | " << *msg.getValue<float>(1) << std::endl;
			m_timeTable.addTime(*msg.getValue<std::string>(0), *msg.getValue<float>(1));
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

			//Check if the online mode = running to prevnt the game from a wrong start
			//In singelplayer onlinemode is always running
			//Check for respawn etc.
			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Return && m_onlineState == OnlineState::RUNNING)
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
							quitOnlineGame();
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
							quitOnlineGame();
							m_music.stop();
							m_active = false;
						}
					}
				}
				else if (event.key.code == sf::Keyboard::BackSpace && m_onlineState == OnlineState::RUNNING)
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
						quitOnlineGame();
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

			
			// 0 = level 1-5
			// 2 = level 6-10
			// 3 = level 11-15

			if (levelnumber < 6)
			{
				if (m_openMusic != 0)
					m_music.openFromFile("data/music/Galaxy - New Electro House Techno by MafiaFLairBeatz.ogg");

				m_openMusic = 0;
			}
			else if (levelnumber < 11)
			{
				if (m_openMusic != 1)
					m_music.openFromFile("data/music/MachinimaSound.com_-_After_Dark.ogg");

				m_openMusic = 1;
			}
			else
			{
				m_music.openFromFile("data/music/Galaxy - New Electro House Techno by MafiaFLairBeatz.ogg");
			}
		}
	}

	void Game::loadLevel()
	{
		//Reset all members first
		m_gameState = GameState::STOPPED;
		m_mapRenderer = MapRenderer();
		m_collisionSystem = CollisionSystem();
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
		m_players[0].loadInformation(path);
		//Call player update with 0 frametime to prevent a instant death.
		m_players[0].update(sf::Time());
		//Update Camera position
		m_camera.update(m_players[0].getPosition());
		//Load all the scripts
		m_scriptManager.load(path);
		//Open the right Music
		initMusic();
		//Reset the gameTimer
		m_gameTimer.restart();
	}


	void Game::resetToLastCheckpoint()
	{
		auto ptr = m_scriptManager.getLastCheckPoint();
		if (ptr)
		{
			//Triggered checkpoint found -> reset to it
			m_players[0] = ptr->savedPlayer;
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
		m_players[0].resetToStartSettings();

		//Reset Camera
		m_camera = Camera();

		//Reset scripts
		m_scriptManager.resetScriptStates();

		//Reset the gameTimer
		m_gameTimer.restart();
	}

	void Game::sendInformationLevelFinished(bool startNextLevel)
	{
		Message msg;
		msg.ID = std::hash<std::string>()("level complete");
		msg.push_back(m_levelName);
		msg.push_back(startNextLevel);
		m_messageBus.sendMessage(msg);
	}

	void Game::quitOnlineGame()
	{
		if (m_gameType == GameType::ONLINE_TIME_CHALLENGE)
		{
			Message msg;
			msg.ID = std::hash<std::string>()("quit online game");

			m_messageBus.sendMessage(msg);
		}
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