#include "player.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <fstream>
#include <sstream>

//#define NDEBUG
#include <assert.h>

namespace aw
{
	Player::Player(const std::string &name) :
		m_name(name),
		m_playerState(PlayerState::STOPPED),
		m_spacePressed(false),
		m_leftPressed(false),
		m_rightPressed(false)
	{

	}

	void Player::update(const sf::Time &frameTime, bool timeChallenge)
	{
		if (m_playerState == PlayerState::FLYING)
		{
			//let the gravitation do his work
			if (m_spacePressed)
			{
				m_speed.y -= 1.5f * (m_gravitation * frameTime.asSeconds());
			}
			else
			{
				m_speed.y += (m_gravitation * frameTime.asSeconds());
			}
			if (timeChallenge)
			{
				if (m_leftPressed)
				{
					m_speed.x -= 200.f * frameTime.asSeconds();
				}
				if (m_rightPressed)
				{
					m_speed.x += 200.f * frameTime.asSeconds();
				}
			}
			//update the position
			m_position += (m_speed * frameTime.asSeconds());
			//update the body
			updateBody();
		}
	}

	void Player::render(sf::RenderWindow &window)
	{
		window.draw(&m_body[0], 3, sf::Triangles);
	}

	void Player::loadInformation(const std::string &path)
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
				m_speed.x = startSpeed;
				m_startXSpeed = m_speed.x;
			}
			else if (line == "[Start Gravitation]")
			{
				std::getline(file, line);
				std::stringstream sstr(line);
				float startGravitation;
				sstr >> startGravitation;
				m_gravitation = startGravitation;
				m_startGravitation = startGravitation;
			}
			else if (line == "[Start Position]")
			{
				std::getline(file, line);
				std::stringstream sstr(line);
				sf::Vector2f startPosition;
				sstr >> startPosition.x >> startPosition.y;
				m_position = startPosition * 25.f; // *25 because of the block size...
				m_startPosition = m_position;
			}
		}

		m_bodyColor = sf::Color::White;

		file.close();
	}

	void Player::resetToStartSettings()
	{
		m_position = m_startPosition;
		m_speed.x = m_startXSpeed;
		m_speed.y = 0;
		m_gravitation = m_startGravitation;

		updateBody();
	}

	void Player::setPosition(const sf::Vector2f &position)
	{
		m_position = position;
	}

	void Player::setSpeedX(float xSpeed)
	{
		m_speed.x = xSpeed;
	}

	void Player::setSpeed(const sf::Vector2f speed)
	{
		m_speed = speed;
	}

	void Player::setGravitation(float gravitation)
	{
		m_gravitation = gravitation;
	}

	void Player::setColor(const sf::Color &color)
	{
		m_bodyColor = color;
	}

	void Player::setPlayerState(PlayerState val)
	{
		m_playerState = val;
	}

	void Player::setSpacePressed(bool val)
	{
		m_spacePressed = val;
	}

	void Player::setLeftPressed(bool val)
	{
		m_leftPressed = val;
	}

	void Player::setRightPressed(bool val)
	{
		m_rightPressed = val;
	}

	bool Player::getSpacePressed()
	{
		return m_spacePressed;
	}

	bool Player::getLeftPressed()
	{
		return m_leftPressed;
	}

	bool Player::getRightPressed()
	{
		return m_rightPressed;
	}

	float Player::getXPosition() const
	{
		return m_position.x;
	}
	const sf::Vector2f &Player::getPosition() const
	{
		return m_position;
	}
	const sf::Vector2f &Player::getSpeed() const
	{
		return m_speed;
	}
	float Player::getGravitation() const
	{
		return m_gravitation;
	}

	sf::Vector2f Player::getVertexPosition(std::size_t index) const
	{
		if (index < 3)
		{
			return m_body[index].position;
		}
		assert(index < 3);
		return sf::Vector2f(-1,-1);
	}

	const std::string &Player::getName()
	{
		return m_name;
	}

	void Player::updateBody()
	{
		//Size of the "ship"
		float sizeX = 20;
		float halfSizeY = 6;
		//Needed to calculate the rotation of the triangle
		sf::Vector2f unitSpeed = m_speed / std::sqrt((m_speed.x * m_speed.x) + (m_speed.y * m_speed.y));
		
		m_body[0] = sf::Vertex(sf::Vector2f(m_position.x + (sizeX * unitSpeed.x), m_position.y + (sizeX * unitSpeed.y)), m_bodyColor);
		m_body[1] = sf::Vertex(sf::Vector2f(m_position.x + (halfSizeY * unitSpeed.y), m_position.y + (halfSizeY * -unitSpeed.x)), m_bodyColor);
		m_body[2] = sf::Vertex(sf::Vector2f(m_position.x + -(halfSizeY * unitSpeed.y), m_position.y + -(halfSizeY * -unitSpeed.x)), m_bodyColor);
	}
}