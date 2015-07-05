#include "intro.hpp"

#include "../../global.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

const float TIME_LIMIT = 3.5f; //in seconds


namespace aw
{
	Intro::Intro(StateMachine &stateMachine):
		State(stateMachine), m_timeActive(0)
	{
		m_texLogo.loadFromFile(Helper::getData("images/kroniax.png"));
		m_sprLogo.setTexture(m_texLogo);
		m_sprLogo.setPosition(95, -40);
		m_sprLogo.setColor(sf::Color(255, 255, 255, 255));
	}

	void Intro::update(const sf::Time &frameTime)
	{
		m_timeActive += frameTime.asSeconds();

		if (m_timeActive > TIME_LIMIT)
		{
			changeActiveState("menu");
		}
		else
		{
			int fadeFactor = static_cast<int>(255 - (255 * (m_timeActive / TIME_LIMIT)));
			m_sprLogo.setColor(sf::Color(255, 255, 255, fadeFactor));
		}
	}

	void Intro::render(sf::RenderWindow &window)
	{
		window.draw(m_sprLogo);
	}
}