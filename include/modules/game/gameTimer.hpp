#ifndef AWGAMETIMER_HPP
#define AWGAMETIMER_HPP

#include <SFML\Graphics\Font.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sf
{
	class RenderWindow;
}

namespace aw
{
	class GameTimer
	{
	public:
		GameTimer();

		void update(float frameTimeInSeconds);

		void restart(float startTime = 0);

		void render(sf::RenderWindow &window);

	private:

	private:
		float m_elapsedTime;

		sf::Font m_font;
		sf::Text m_text;
	};
}

#endif