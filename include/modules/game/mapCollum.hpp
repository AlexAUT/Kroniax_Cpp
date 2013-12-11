#ifndef AWMAPCOLLUM_HPP
#define AWMAPCOLLUM_HPP

/////INCLUDES/////
#include <SFML/Graphics/VertexArray.hpp>

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
		MapCollum();

		void addRect(const sf::Vector2f &position, const sf::Vector2f &size, const sf::Color &color);

		void render(sf::RenderWindow &window);

	private:

	private:
		sf::VertexArray m_vertexArray;
	};
}

#endif //AWMAPCOLLUM_HPP