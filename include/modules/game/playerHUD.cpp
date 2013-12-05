#include "playerHUD.hpp"

#include "player.hpp"

#include <sstream>

#include <SFML/Graphics/RenderWindow.hpp>

namespace aw
{
	PlayerHUD::PlayerHUD()
	{
		m_font.loadFromFile("data/fonts/visitor1.ttf");
		m_displaySpeed.setFont(m_font);
		m_displaySpeed.setColor(sf::Color::White);
		m_displaySpeed.setCharacterSize(18);
		
		m_displayGravity = m_displaySpeed;

		m_displaySpeed.setPosition(sf::Vector2f(5, 5));
		m_displayGravity.setPosition(sf::Vector2f(5, 25));
	}

	void PlayerHUD::update(const Player &player)
	{
		//static_cast<int> to avoid showing ugly floating numbers...
		std::stringstream sstr;
		sstr << "Speed: " << static_cast<int>(player.getSpeed().x);
		m_displaySpeed.setString(sstr.str());

		sstr.str("");
		sstr << "Gravity: " << static_cast<int>(player.getGravitation());
		m_displayGravity.setString(sstr.str());
	}

	void PlayerHUD::render(sf::RenderWindow &window)
	{
		window.draw(m_displaySpeed);
		window.draw(m_displayGravity);
	}
}