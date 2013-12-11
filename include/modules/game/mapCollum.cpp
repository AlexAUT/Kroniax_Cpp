#include "mapCollum.hpp"

/////INCLUDES/////
#include <SFML/Graphics/RenderWindow.hpp>

namespace aw
{
	MapCollum::MapCollum() :
		m_vertexArray(sf::Triangles)
	{

	}

	void MapCollum::addRect(const sf::Vector2f &position, const sf::Vector2f &size, const sf::Color &color)
	{
		//Triangle1:
		//Left top
		m_vertexArray.append(sf::Vertex(sf::Vector2f(position), color));
		//Right top
		m_vertexArray.append(sf::Vertex(sf::Vector2f(position.x + size.x, position.y), color));
		//Left bottom
		m_vertexArray.append(sf::Vertex(sf::Vector2f(position.x, position.y + size.y), color));
		
		//Triangle2:
		//Right top
		m_vertexArray.append(m_vertexArray[m_vertexArray.getVertexCount() - 2]);
		//Right bottom
		m_vertexArray.append(sf::Vertex(sf::Vector2f(position + size), color));
		//Left bottom
		m_vertexArray.append(m_vertexArray[m_vertexArray.getVertexCount() - 3]);
	}

	void MapCollum::render(sf::RenderWindow &window)
	{
		window.draw(m_vertexArray);
	}
}