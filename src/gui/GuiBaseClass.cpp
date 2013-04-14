#include "../../include/gui/GuiBaseClass.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

GuiBaseElement::GuiBaseElement()
{
    m_activeEntry = -1;
}

GuiBaseElement::GuiBaseElement(GuiType type, const std::string& ID, const sf::Vector2f& position, const std::string& text):
    m_type(type),
    m_selected(false),
    m_selectable(type != GUI_LABEL), // Labels are not selectable
    m_ID(ID),
    m_position(position)
{
    m_body.setString(text);
    m_body.setPosition(position);
    m_body.setCharacterSize(16);
    m_activeEntry = -1;
}

void GuiBaseElement::HandleEvent(const sf::Event &e)
{
    switch (m_type)
    {
    case GUI_LIST:
        if(e.type == sf::Event::KeyPressed)
        {
            if(e.key.code == sf::Keyboard::Left)
            {
                if(m_activeEntry <= 0)
                {
                    m_activeEntry = m_entries.size()-1;
                }
                else
                {
                    m_activeEntry--;
                }

                if(m_activeEntry >= 0)
                    m_body.setString(m_entries[m_activeEntry]);

                CreateSelectionVertices();

            }

            if(e.key.code == sf::Keyboard::Right)
            {
                if(m_activeEntry >= static_cast<int>(m_entries.size()-1))
                {
                    m_activeEntry = 0;
                }
                else
                {
                    m_activeEntry++;
                }

                if(m_activeEntry < static_cast<int>(m_entries.size()))
                    m_body.setString(m_entries[m_activeEntry]);

                CreateSelectionVertices();

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
                CreateSelectionVertices();
            }
        }
        else if (e.type == sf::Event::KeyPressed)
        {
             //Delete
            if(e.key.code == sf::Keyboard::BackSpace)
            {
                sf::String temp = m_body.getString();
                if(temp.getSize() > 0)
                {
                    temp.erase(temp.getSize() - 1, 1);
                    m_body.setString(temp);
                }
                CreateSelectionVertices();
            }
        }
        break;

    default:
        break;
    }
}

void GuiBaseElement::Draw(sf::RenderWindow &window) const
{
    if(m_selected)
        window.draw(m_vertices, 6, sf::Triangles);

    window.draw(m_body);

}

void GuiBaseElement::AddEntry(std::string entry)
{
    m_entries.push_back(entry);
}

void GuiBaseElement::ClearEntries()
{
    m_entries.clear();
}

//
// Getter
//


const std::string& GuiBaseElement::GetID() const
{
    return m_ID;
}

const sf::Vector2f& GuiBaseElement::GetPosition() const
{
    return m_position;
}

std::string GuiBaseElement::GetText() const
{
    return m_body.getString();
}

const sf::Text& GuiBaseElement::GetTextObj() const
{
    return m_body;
}

bool GuiBaseElement::IsSelectable() const
{
    return m_selectable;
}

//
// Setter
//

void GuiBaseElement::SetID(std::string ID)
{
    m_ID = ID;
}

void GuiBaseElement::SetSelected(bool value)
{
    m_selected = value;

    if(value)
    {
        m_body.setColor(sf::Color::Red);
    }
    else
    {
        m_body.setColor(sf::Color::White);
    }
}

void GuiBaseElement::SetSelectable(bool value)
{
    m_selectable = value;
}

void GuiBaseElement::SetFont(const sf::Font &font)
{
    m_body.setFont(font);

    CreateSelectionVertices();
}

void GuiBaseElement::SetPosition(sf::Vector2f position)
{
    m_position = position;

    m_body.setPosition(m_position);

    CreateSelectionVertices();
}

void GuiBaseElement::SetX(float x)
{
    m_position.x = x;
    m_body.setPosition(m_position);

    CreateSelectionVertices();
}

void GuiBaseElement::SetY(float y)
{
    m_position.y = y;
    m_body.setPosition(m_position);

    CreateSelectionVertices();
}

void GuiBaseElement::SetText(std::string text)
{
    m_body.setString(text);

    CreateSelectionVertices();
}

void GuiBaseElement::SetActiveEntry(unsigned int index)
{
    if(index < m_entries.size())
    {
        m_activeEntry = index;
        m_body.setString(m_entries[m_activeEntry]);
        CreateSelectionVertices();
    }
}

void GuiBaseElement::SetCharacterSize(unsigned int size)
{
    m_body.setCharacterSize(size);
    CreateSelectionVertices();
}

void GuiBaseElement::SetObjectToCenter(int windowWidth)
{
    m_position.x = (windowWidth / 2) - (m_body.getGlobalBounds().width / 2);

    SetPosition(m_position);
}



//
// Private Functions
//

void GuiBaseElement::CreateSelectionVertices()
{
    float xPos = m_body.getPosition().x;
    float yPos = m_body.getPosition().y;
    float width = m_body.getGlobalBounds().width;
    float height = m_body.getGlobalBounds().height;

    float distance = 8.f;
    float triWidth = 10;
    float triheight = 10;

    float offset = 3.f;

    if(m_type == GUI_BUTTON)
    {
        // Left triangle
        m_vertices[0] = sf::Vertex(sf::Vector2f(xPos - distance - triWidth, yPos + (height/2.f) - (triheight/2.f) + offset), sf::Color::Red); // Left Top
        m_vertices[1] = sf::Vertex(sf::Vector2f(xPos - distance - triWidth, yPos + (height/2.f) + (triheight/2.f) + offset), sf::Color::Red); // Left Bot
        m_vertices[2] = sf::Vertex(sf::Vector2f(xPos - distance           , yPos + (height/2.f)                   + offset), sf::Color::Red); // Right Edge


        // Right triangle
        m_vertices[3] = sf::Vertex(sf::Vector2f(xPos + width + distance + triWidth, yPos + (height/2.f) - (triheight/2.f) + offset), sf::Color::Red); // Right Top
        m_vertices[4] = sf::Vertex(sf::Vector2f(xPos + width + distance + triWidth, yPos + (height/2.f) + (triheight/2.f) + offset), sf::Color::Red); // Right Bot
        m_vertices[5] = sf::Vertex(sf::Vector2f(xPos + width + distance           , yPos + (height/2.f)                   + offset), sf::Color::Red); // Left Edge
    }

    if(m_type == GUI_LIST)
    {
        // Left triangle
        m_vertices[0] = sf::Vertex(sf::Vector2f(xPos - distance           , yPos + (height/2.f) - (triheight/2.f) + offset), sf::Color::Red); // Left Top
        m_vertices[1] = sf::Vertex(sf::Vector2f(xPos - distance           , yPos + (height/2.f) + (triheight/2.f) + offset), sf::Color::Red); // Left Bot
        m_vertices[2] = sf::Vertex(sf::Vector2f(xPos - distance - triWidth, yPos + (height/2.f)                   + offset), sf::Color::Red); // Right Edge


        // Right triangle
        m_vertices[3] = sf::Vertex(sf::Vector2f(xPos + width + distance           , yPos + (height/2.f) - (triheight/2.f) + offset), sf::Color::Red); // Right Top
        m_vertices[4] = sf::Vertex(sf::Vector2f(xPos + width + distance           , yPos + (height/2.f) + (triheight/2.f) + offset), sf::Color::Red); // Right Bot
        m_vertices[5] = sf::Vertex(sf::Vector2f(xPos + width + distance + triWidth, yPos + (height/2.f)                   + offset), sf::Color::Red); // Left Edge
    }

    if(m_type == GUI_INPUT) // Recht surrounding the Text
    {
        m_vertices[0] = sf::Vertex(sf::Vector2f(xPos - distance, yPos - distance / 3), sf::Color(165,165,165)); // LeftTop
        m_vertices[1] = sf::Vertex(sf::Vector2f(xPos + width + distance, yPos - distance/3), sf::Color(165,165,165)); // RightTop
        m_vertices[2] = sf::Vertex(sf::Vector2f(xPos + width + distance, yPos + height + distance), sf::Color(165,165,165)); // RightBttom

        m_vertices[3] = m_vertices[2]; // RightBttom
        m_vertices[4] = sf::Vertex(sf::Vector2f(xPos - distance, yPos + height + distance), sf::Color(165,165,165)); // LeftBottom
        m_vertices[5] = m_vertices[0]; // LeftTop
    }
}
