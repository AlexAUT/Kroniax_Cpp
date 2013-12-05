#ifndef AWPLAYERHUD_HPP
#define AWPLAYERHUD_HPP

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sf
{
	class RenderWindow;
}

namespace aw
{
	class Player;

	class PlayerHUD
	{
	public:
		PlayerHUD();

		void update(const Player &player);
		void render(sf::RenderWindow &window);

	private:

	private:
		sf::Font m_font;
		sf::Text m_displaySpeed;
		sf::Text m_displayGravity;
	};
}

#endif