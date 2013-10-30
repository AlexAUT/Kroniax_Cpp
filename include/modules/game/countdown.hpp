#ifndef AWCOUNTDOWN_HPP
#define AWCOUNTDOWN_HPP

#include <string>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sf
{
	class RenderWindow;
}

namespace aw
{
	class Countdown
	{
	public:
		Countdown();

		void setTime(float time);

		void update(float frameTimeInSeconds);

		void render(sf::RenderWindow &window, const std::string &addText = "");

	private:

	private:

		float m_timeLeft;

		sf::Font m_font;
		sf::Text m_text;

	};
}

#endif