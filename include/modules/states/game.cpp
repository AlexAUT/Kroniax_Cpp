#include "game.hpp"

#include <fstream>
#include <sstream>

#include <iostream>

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
		//Update player position
		m_player.upate(frameTime);
		//check for collision or finish
		auto result = m_collisionSystem.checkCollision(m_player);
		if (result == CollisionType::WALL)
		{
			std::cout << "Wall!!";
		}
		else if (result == CollisionType::FINISH)
		{
			std::cout << "Finish!!";
		}
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
		std::cout << "levelpath: " << path << std::endl;
		//Load the mapRenderer
		m_mapRenderer.load(path);
		m_collisionSystem.loadMap(path);
		m_player.loadInformation(path);
	}

}