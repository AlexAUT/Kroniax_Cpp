#include "game.hpp"

#include <fstream>
#include <sstream>

#include <SFML/Graphics/RenderWindow.hpp>

#include "../../messageBus/messageBus.hpp"

namespace aw
{
	Game::Game(StateMachine &statemachine, MessageBus &messageBus):
		m_active(false),
		State(statemachine), 
		m_messageBus(messageBus)
	{
		m_messageBus.addReceiver(this);
	}

	void Game::update(const sf::Time &frameTime)
	{
		m_active = true;

		//check for collision or finish
		auto result = m_collisionSystem.checkCollision(m_player);
		if (result == CollisionType::WALL)
		{
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
	}

	void Game::receiveMessage(const Message &msg)
	{
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
	}


	////////// PRIVATE FUNCS ////////////

	void Game::loadLevel()
	{
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