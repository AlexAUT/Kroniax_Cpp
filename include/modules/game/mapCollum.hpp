#ifndef AWMAPCOLLUM_HPP
#define AWMAPCOLLUM_HPP

/////INCLUDES/////
#include <SFML/Graphics/RectangleShape.hpp>

#include <vector>

/////FORWARD DECL/////
namespace sf
{
	class RenderWindow;
}

namespace aw
{
	class MapCollum
	{
	public:
		void addRect(const sf::Vector2f &position, const sf::Vector2f &size, const sf::Color &color);

		void render(sf::RenderWindow &window);

	private:

	private:
		std::vector<sf::RectangleShape> m_rects;
	};
}

#endif //AWMAPCOLLUM_HPP