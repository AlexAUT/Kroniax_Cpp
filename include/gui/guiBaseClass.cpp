#include "../../include/gui/guiBaseClass.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace aw
{

	GuiBaseElement::GuiBaseElement()
	{
		m_activeEntry = -1;
	}

	GuiBaseElement::GuiBaseElement(GuiType type, const std::string& ID, const sf::Vector2f& position, const std::string& text) :
		m_type(type),
		m_selected(false),
		m_selectable(type != GUI_LABEL), // Labels are not selectable
		m_ID(ID),
		m_position(position),
		m_selectColor(sf::Color(255, 165, 0))
	{
		m_body.setString(text);
		m_body.setPosition(position);
		m_body.setCharacterSize(23);
		m_activeEntry = -1;
	}

	void GuiBaseElement::handleEvent(const sf::Event &e)
	{
		switch (m_type)
		{        
		case GUI_LIST:
			if (e.type == sf::Event::KeyPressed)
			{
				if (e.key.code == sf::Keyboard::Left)
				{
					if (m_activeEntry <= 0)
					{
						m_activeEntry = m_entries.size() - 1;
					}
					else
					{
						m_activeEntry--;
					}

					if (m_activeEntry >= 0)
						m_body.setString(m_entries[m_activeEntry]);

					createSelectionVertices();

				}

				if (e.key.code == sf::Keyboard::Right)
				{
					if (m_activeEntry >= static_cast<int>(m_entries.size() - 1))
					{
						m_activeEntry = 0;
					}
					else
					{
						m_activeEntry++;
					}

					if (m_activeEntry < static_cast<int>(m_entries.size()))
						m_body.setString(m_entries[m_activeEntry]);

					createSelectionVertices();

				}
			}
			break;

		case GUI_INPUT:
			if (e.type == sf::Event::TextEntered)
			{
				sf::Uint32 unicode = e.text.unicode;
				// Ensure the unicode character is printable
				if (unicode > 30 && (unicode < 127 || unicode > 159))
				{
					sf::String string = m_body.getString();
					string += unicode;
					m_body.setString(string);
					createSelectionVertices();
				}
			}
			else if (e.type == sf::Event::KeyPressed)
			{
				//Delete
				if (e.key.code == sf::Keyboard::BackSpace)
				{
					sf::String temp = m_body.getString();
					if (temp.getSize() > 0)
					{
						temp.erase(temp.getSize() - 1, 1);
						m_body.setString(temp);
					}
					createSelectionVertices();
				}
			}
			break;
        default: break;
		}
	}

	void GuiBaseElement::render(sf::RenderWindow &window) const
	{
		if (m_selected)
			window.draw(m_vertices, 6, sf::Triangles);

		window.draw(m_body);

	}

	void GuiBaseElement::addEntry(std::string entry)
	{
		m_entries.push_back(entry);
	}

	void GuiBaseElement::clearEntries()
	{
		m_entries.clear();
	}

	//
	// Getter
	//


	const std::string& GuiBaseElement::getID() const
	{
		return m_ID;
	}

	const sf::Vector2f& GuiBaseElement::getPosition() const
	{
		return m_position;
	}

	std::string GuiBaseElement::getText() const
	{
		return m_body.getString();
	}

	const sf::Text& GuiBaseElement::getTextObj() const
	{
		return m_body;
	}

	bool GuiBaseElement::isSelectable() const
	{
		return m_selectable;
	}

	//
	// Setter
	//

	void GuiBaseElement::setID(std::string ID)
	{
		m_ID = ID;
	}

	void GuiBaseElement::setSelected(bool value)
	{
		m_selected = value;

		if (value)
		{
			m_body.setColor(m_selectColor);
		}
		else
		{
			m_body.setColor(sf::Color::White);
		}
	}

	void GuiBaseElement::setSelectable(bool value)
	{
		m_selectable = value;
	}

	void GuiBaseElement::setFont(const sf::Font &font)
	{
		m_body.setFont(font);

		createSelectionVertices();
	}

	void GuiBaseElement::setPosition(sf::Vector2f position)
	{
		m_position = position;

		m_body.setPosition(m_position);

		createSelectionVertices();
	}

	void GuiBaseElement::setX(float x)
	{
		m_position.x = x;
		m_body.setPosition(m_position);

		createSelectionVertices();
	}

	void GuiBaseElement::setY(float y)
	{
		m_position.y = y;
		m_body.setPosition(m_position);

		createSelectionVertices();
	}

	void GuiBaseElement::setText(std::string text)
	{
		m_body.setString(text);

		createSelectionVertices();
	}

	void GuiBaseElement::setActiveEntry(unsigned int index)
	{
		if (index < m_entries.size())
		{
			m_activeEntry = index;
			m_body.setString(m_entries[m_activeEntry]);
			createSelectionVertices();
		}
	}

	void GuiBaseElement::setCharacterSize(unsigned int size)
	{
		m_body.setCharacterSize(size);
		createSelectionVertices();
	}

	void GuiBaseElement::setObjectToCenter(int windowWidth)
	{
		m_position.x = (windowWidth / 2) - (m_body.getGlobalBounds().width / 2);

		setPosition(m_position);
	}



	//
	// Private Functions
	//

	void GuiBaseElement::createSelectionVertices()
	{
		float xPos = m_body.getPosition().x;
		float yPos = m_body.getPosition().y;
		float width = m_body.getGlobalBounds().width;
		float height = m_body.getGlobalBounds().height;

		float distance = 8.f;
		float triWidth = 10;
		float triheight = 10;

		float offset = 8.f;

		if (m_type == GUI_BUTTON)
		{
			// Left triangle
			m_vertices[0] = sf::Vertex(sf::Vector2f(xPos - distance - triWidth, yPos + (height / 2.f) - (triheight / 2.f) + offset), m_selectColor); // Left Top
			m_vertices[1] = sf::Vertex(sf::Vector2f(xPos - distance - triWidth, yPos + (height / 2.f) + (triheight / 2.f) + offset), m_selectColor); // Left Bot
			m_vertices[2] = sf::Vertex(sf::Vector2f(xPos - distance, yPos + (height / 2.f) + offset), m_selectColor); // Right Edge


			// Right triangle
			m_vertices[3] = sf::Vertex(sf::Vector2f(xPos + width + distance + triWidth, yPos + (height / 2.f) - (triheight / 2.f) + offset), m_selectColor); // Right Top
			m_vertices[4] = sf::Vertex(sf::Vector2f(xPos + width + distance + triWidth, yPos + (height / 2.f) + (triheight / 2.f) + offset), m_selectColor); // Right Bot
			m_vertices[5] = sf::Vertex(sf::Vector2f(xPos + width + distance, yPos + (height / 2.f) + offset), m_selectColor); // Left Edge
		}

		if (m_type == GUI_LIST)
		{
			// Left triangle
			m_vertices[0] = sf::Vertex(sf::Vector2f(xPos - distance, yPos + (height / 2.f) - (triheight / 2.f) + offset), m_selectColor); // Left Top
			m_vertices[1] = sf::Vertex(sf::Vector2f(xPos - distance, yPos + (height / 2.f) + (triheight / 2.f) + offset), m_selectColor); // Left Bot
			m_vertices[2] = sf::Vertex(sf::Vector2f(xPos - distance - triWidth, yPos + (height / 2.f) + offset), m_selectColor); // Right Edge


			// Right triangle
			m_vertices[3] = sf::Vertex(sf::Vector2f(xPos + width + distance, yPos + (height / 2.f) - (triheight / 2.f) + offset), m_selectColor); // Right Top
			m_vertices[4] = sf::Vertex(sf::Vector2f(xPos + width + distance, yPos + (height / 2.f) + (triheight / 2.f) + offset), m_selectColor); // Right Bot
			m_vertices[5] = sf::Vertex(sf::Vector2f(xPos + width + distance + triWidth, yPos + (height / 2.f) + offset), m_selectColor); // Left Edge
		}

		if (m_type == GUI_INPUT) // Rect surrounding the Text
		{
			m_vertices[0] = sf::Vertex(sf::Vector2f(xPos - distance, yPos - distance / 3), sf::Color(165, 165, 165)); // LeftTop
			m_vertices[1] = sf::Vertex(sf::Vector2f(xPos + width + distance, yPos - distance / 3), sf::Color(165, 165, 165)); // RightTop
			m_vertices[2] = sf::Vertex(sf::Vector2f(xPos + width + distance, yPos + height + distance), sf::Color(165, 165, 165)); // RightBttom

			m_vertices[3] = m_vertices[2]; // RightBttom
			m_vertices[4] = sf::Vertex(sf::Vector2f(xPos - distance, yPos + height + distance), sf::Color(165, 165, 165)); // LeftBottom
			m_vertices[5] = m_vertices[0]; // LeftTop
		}
	}

}
