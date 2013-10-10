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

		if (m_gameState == GameState::RUNNING)
		{
			//check for collision or finish
			auto result = m_collisionSystem.checkCollision(m_player);
			if (result == CollisionType::WALL)
			{
				m_gameState = GameState::STOPPED;
				resetToLastCheckpoint();
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
			//Set the gametype
			if (*msg.getValue<std::string>(1) == "official arcade")
			{
				m_gameType = GameType::OFFICIAL_ARCADE;
			}
			//Load the level
			loadLevel();
		}
		//Starting the game
		if (msg.ID == std::hash<std::string>()("event") && m_active)
		{
			sf::Event event = *msg.getValue<sf::Event>(0);

			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Return)
				{
					if (m_gameState == GameState::STOPPED)
					{
						m_gameState = GameState::RUNNING;
					}
				}
			}
		}
	}


	////////// PRIVATE FUNCS ////////////

	void Game::loadLevel()
	{
		//Reset all modules first
		m_mapRenderer = MapRenderer();
		m_collisionSystem = CollisionSystem();
		m_player = Player();
		m_camera = Camera();

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
	}


	void Game::resetToLastCheckpoint()
	{
		auto ptr = m_scriptManager.getLastCheckPoint();
		if (ptr)
		{
			m_player = ptr->savedPlayer;
			m_camera = ptr->savedCamera;
		}
		else
		{
			//Restart Level
			loadLevel();
		}
	}
}



//GLOBAL FUNCTION

void initGui(aw::GuiController &gui)
{
	//Stopped Layer
	//How to start the game
	gui.addLayer();

	gui.addButton(0, "msg", sf::Vector2f(75, 135), "Enter: Start the Game from checkpoint");
	gui.getElement(0, 0)->setCharacterSize(30);
	gui.getElement(0, 0)->setSelectable(false);
	gui.getElement(0, 0)->setSelected(false);
	gui.addButton(0, "msg2", sf::Vector2f(175, 250), "Delete: Start the game from Start");
	gui.getElement(0, 1)->setCharacterSize(20);
	gui.getElement(0, 1)->setSelectable(false);
	gui.setActiveLayer(0);
}