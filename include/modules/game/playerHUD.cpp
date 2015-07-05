#include "playerHUD.hpp"

#include "player.hpp"
#include "../../global.hpp"

#include <sstream>

#include <SFML/Graphics/RenderWindow.hpp>

namespace aw
{
	PlayerHUD::PlayerHUD()
	{
		m_font.loadFromFile(Helper::getData("fonts/visitor1.ttf"));
		m_displaySpeed.setFont(m_font);
		m_displaySpeed.setColor(sf::Color::White);
		m_displaySpeed.setCharacterSize(18);
		
		m_displayGravity = m_displaySpeed;

		m_displaySpeed.setPosition(sf::Vector2f(5, 5));
		m_displayGravity.setPosition(sf::Vector2f(5, 25));
	}

	void PlayerHUD::update(const Player &player)
	{
		//std::round to avoid showing ugly floating numbers...
		std::stringstream sstr;
		sstr << "Speed: " << std::round(player.getSpeed().x);
		m_displaySpeed.setString(sstr.str());

		sstr.str("");
		sstr << "Gravity: " << std::round(player.getGravitation());
		m_displayGravity.setString(sstr.str());
	}

	void PlayerHUD::render(sf::RenderWindow &window)
	{
		window.draw(m_displaySpeed);
		window.draw(m_displayGravity);
	}
}