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
	class Player
	{
	public:

		void update(const sf::Time &frameTime);
		void render(sf::RenderWindow &window);

		void loadInformation(const std::string &path);

		void resetToStartSettings();

		void setPosition(const sf::Vector2f &position);
		void setSpeedX(float xSpeed);
		void setGravitation(float gravitation);
		void setColor(const sf::Color &color);

		float getXPosition() const;
		const sf::Vector2f &getPosition() const;
		sf::Vector2f getVertexPosition(std::size_t index) const;

	private:

		void updateBody();

	private:
		//Propterties
		sf::Vector2f m_position;
		sf::Vector2f m_speed;
		float m_gravitation;
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