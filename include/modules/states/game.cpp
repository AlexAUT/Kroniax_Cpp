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

		m_player.upate(frameTime);
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
		loadPlayerInformation(path);
	}

	void Game::loadPlayerInformation(const std::string &path)
	{
		std::fstream file(path.c_str(), std::ios::in);

		std::string line;
		while (std::getline(file, line))
		{
			if (line == "[Start speed]")
			{
				std::getline(file, line);
				std::stringstream sstr(line);
				float startSpeed;
				sstr >> startSpeed;
				m_player.setSpeedX(startSpeed);
			}
			else if (line == "[Start Gravitation]")
			{
				std::getline(file, line);
				std::stringstream sstr(line);
				float startGravitation;
				sstr >> startGravitation;
				m_player.setGravitation(startGravitation);
			}
			else if (line == "[Start Position]")
			{
				std::getline(file, line);
				std::stringstream sstr(line);
				sf::Vector2f startPosition;
				sstr >> startPosition.x >> startPosition.y;
				m_player.setPosition(startPosition * 25.f); // *25 because of the block size...
			}
		}

		m_player.setColor(sf::Color::White);

		file.close();
	}
}