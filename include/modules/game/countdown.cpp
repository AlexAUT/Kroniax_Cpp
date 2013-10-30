#include "countdown.hpp"

#include <sstream>

#include <SFML/Graphics/RenderWindow.hpp>


namespace aw
{
	Countdown::Countdown() :
		m_timeLeft(0.f)
	{
		m_font.loadFromFile("data/fonts/visitor1.ttf");
		m_text.setFont(m_font);
		m_text.setPosition(300, 5);
		m_text.setCharacterSize(15);
	}

	void Countdown::setTime(float time)
	{
		m_timeLeft = time;
	}

	void Countdown::update(float frameTimeInSeconds)
	{
		if (m_timeLeft > 0)
			m_timeLeft -= frameTimeInSeconds;
	}

	void Countdown::render(sf::RenderWindow &window, const std::string &addText)
	{
		//Round to 2digits to display...
		int roundInt = static_cast<int>(m_timeLeft);

		std::stringstream sstr;
		sstr << roundInt;

		m_text.setString(sstr.str() + " sec left " + addText);

		window.draw(m_text);
	}
}