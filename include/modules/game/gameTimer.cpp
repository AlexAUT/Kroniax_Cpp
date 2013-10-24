#include "gameTimer.hpp"

#include <sstream>

#include <SFML/Graphics/RenderWindow.hpp>

namespace aw
{
	GameTimer::GameTimer() :
		m_elapsedTime(0)
	{
		m_font.loadFromFile("data/fonts/visitor1.ttf");
		m_text.setFont(m_font);
		m_text.setPosition(10, 415);
		m_text.setCharacterSize(23);
		m_text.setString("Time: ");
	}

	void GameTimer::update(float frameTimeInSeconds)
	{
		m_elapsedTime += frameTimeInSeconds;

		//Round to 2digits to display...
		int roundInt = static_cast<int>(m_elapsedTime * 100.f);
		float roundFloat = roundInt / 100.f;

		std::stringstream sstr;
		sstr << roundFloat;

		m_text.setString("Time: " + sstr.str());
	}

	void GameTimer::restart(float elapsedTime)
	{
		m_elapsedTime = elapsedTime;
	}

	void GameTimer::render(sf::RenderWindow &window)
	{
		window.draw(m_text);
	}


	float GameTimer::getTime()
	{
		return m_elapsedTime;
	}
}