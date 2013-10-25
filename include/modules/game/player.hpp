#ifndef AWPLAYER_HPP
#define AWPLAYER_HPP

//////INCLUDES///////
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Color.hpp>

#include <array>
#include <string>

//////FORWARD DECL//////
namespace sf
{
	class Time;
	class RenderWindow;
}

namespace aw
{
	enum PlayerState
	{
		STOPPED,
		FLYING,
	};

	class Player
	{
	public:

		Player(const std::string &name = "");

		void update(const sf::Time &frameTime, bool timeChallenge = false);
		void render(sf::RenderWindow &window);

		void loadInformation(const std::string &path);

		void resetToStartSettings();

		void setPosition(const sf::Vector2f &position);
		void setSpeedX(float xSpeed);
		void setSpeed(const sf::Vector2f speed);
		void setGravitation(float gravitation);
		void setColor(const sf::Color &color);

		void setPlayerState(PlayerState val);

		void setSpacePressed(bool val);
		void setLeftPressed(bool val);
		void setRightPressed(bool val);

		bool getSpacePressed();
		bool getLeftPressed();
		bool getRightPressed();

		float getXPosition() const;
		const sf::Vector2f &getPosition() const;
		const sf::Vector2f &getSpeed() const;
		float getGravitation() const;
		sf::Vector2f getVertexPosition(std::size_t index) const;

		const std::string &getName();

	private:

		void updateBody();

	private:
		std::string m_name;
		//Propterties
		PlayerState m_playerState;

		sf::Vector2f m_position;
		sf::Vector2f m_speed;
		float m_gravitation;
		//Key states
		bool m_spacePressed;
		bool m_leftPressed;
		bool m_rightPressed;
		//Optical Elements
		std::array<sf::Vertex, 3> m_body;
		sf::Color m_bodyColor;

		//Default settings (used to reset to levelstart)
		sf::Vector2f m_startPosition;
		float m_startGravitation;
		float m_startXSpeed;
	};
}

#endif //AWPLAYER_HPP