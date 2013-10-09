#include "mapCollum.hpp"

/////INCLUDES/////
#include <SFML/Graphics/RenderWindow.hpp>

namespace aw
{
	void MapCollum::addRect(const sf::Vector2f &position, const sf::Vector2f &size, const sf::Color &color)
	{
		m_rects.push_back(sf::RectangleShape(size));
		m_rects.back().setPosition(position);
		m_rects.back().setFillColor(color);
	}

	void MapCollum::render(sf::RenderWindow &window)
	{
		for (auto &it : m_rects)
		{
			window.draw(it);
		}
	}
}