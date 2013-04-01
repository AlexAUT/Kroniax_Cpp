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

GuiBaseElement::GuiBaseElement(int type, std::string ID, sf::Vector2f position, std::string text) : m_type(type),  m_selected(false), m_selectAble(true), m_ID(ID), m_position(position)
{
    m_body.setString(text);
    m_body.setPosition(position);
    m_body.setCharacterSize(20);
    m_activeEntry = -1;

}

void GuiBaseElement::HandleEvents(sf::Event &e)
{
    if(m_type == 2)
    {
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
    }

    if(m_type == 3)
    {
        if(e.type == sf::Event::KeyPressed)
            HandleKeyInput(e);
    }
}

void GuiBaseElement::Draw(sf::RenderWindow &window)
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


const std::string& GuiBaseElement::GetID()
{
    return m_ID;
}
const sf::Vector2f& GuiBaseElement::GetPosition()
{
    return m_position;
}
std::string GuiBaseElement::GetText()
{
    return m_body.getString();
}
sf::Text &GuiBaseElement::GetTextObj()
{
    return m_body;
}
bool GuiBaseElement::GetSelectAble()
{
    return m_selectAble;
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
void GuiBaseElement::SetSelectAble(bool value)
{
    m_selectAble = value;
}
void GuiBaseElement::SetFont(sf::Font &font)
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



//
// Private Functions
//

void GuiBaseElement::CreateSelectionVertices()
{
    float xPos = m_body.getPosition().x;
    float yPos = m_body.getPosition().y;
    float width = m_body.getGlobalBounds().width;
    float height = m_body.getGlobalBounds().height;

    float distance = 10.f;
    float triWidth = 0.15f * width;
    float triheight = 0.65f * height;

    float offset = 7.f;

    if(m_type == 1)
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

    if(m_type == 2)
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

    if(m_type == 3) // Recht surrounding the Text
    {
        m_vertices[0] = sf::Vertex(sf::Vector2f(xPos - distance, yPos - distance / 3), sf::Color(165,165,165)); // LeftTop
        m_vertices[1] = sf::Vertex(sf::Vector2f(xPos + width + distance, yPos - distance/3), sf::Color(165,165,165)); // RightTop
        m_vertices[2] = sf::Vertex(sf::Vector2f(xPos + width + distance, yPos + height + distance), sf::Color(165,165,165)); // RightBttom

        m_vertices[3] = m_vertices[2]; // RightBttom
        m_vertices[4] = sf::Vertex(sf::Vector2f(xPos - distance, yPos + height + distance), sf::Color(165,165,165)); // LeftBottom
        m_vertices[5] = m_vertices[0]; // LeftTop
    }
}



void GuiBaseElement::HandleKeyInput(sf::Event &e)
{

    std::string newChar;

    switch (e.key.code)
    {
    case sf::Keyboard::A:
        newChar = "A";
        break;
    case sf::Keyboard::B:
        newChar = "B";
        break;
    case sf::Keyboard::C:
        newChar = "C";
        break;
    case sf::Keyboard::D:
        newChar = "D";
        break;
    case sf::Keyboard::E:
        newChar = "E";
        break;
    case sf::Keyboard::F:
        newChar = "F";
        break;
    case sf::Keyboard::G:
        newChar = "G";
        break;
    case sf::Keyboard::H:
        newChar = "H";
        break;
    case sf::Keyboard::I:
        newChar = "I";
        break;
    case sf::Keyboard::J:
        newChar = "J";
        break;
    case sf::Keyboard::K:
        newChar = "K";
        break;
    case sf::Keyboard::L:
        newChar = "L";
        break;
    case sf::Keyboard::M:
        newChar = "M";
        break;
    case sf::Keyboard::N:
        newChar = "N";
        break;
    case sf::Keyboard::O:
        newChar = "O";
        break;
    case sf::Keyboard::P:
        newChar = "P";
        break;
    case sf::Keyboard::Q:
        newChar = "Q";
        break;
    case sf::Keyboard::R:
        newChar = "R";
        break;
    case sf::Keyboard::S:
        newChar = "S";
        break;
    case sf::Keyboard::T:
        newChar = "T";
        break;
    case sf::Keyboard::U:
        newChar = "U";
        break;
    case sf::Keyboard::V:
        newChar = "V";
        break;
    case sf::Keyboard::W:
        newChar = "W";
        break;
    case sf::Keyboard::X:
        newChar = "X";
        break;
    case sf::Keyboard::Y:
        newChar = "Y";
        break;
    case sf::Keyboard::Z:
        newChar = "Z";
        break;
    case sf::Keyboard::Space:
        newChar = " ";
        break;

    case sf::Keyboard::Key::Num0:
        newChar = "0";
        break;
    case sf::Keyboard::Key::Num1:
        newChar = "1";
        break;
    case sf::Keyboard::Key::Num2:
        newChar = "2";
        break;
    case sf::Keyboard::Key::Num3:
        newChar = "3";
        break;
    case sf::Keyboard::Key::Num4:
        newChar = "4";
        break;
    case sf::Keyboard::Key::Num5:
        newChar = "5";
        break;
    case sf::Keyboard::Key::Num6:
        newChar = "6";
        break;
    case sf::Keyboard::Key::Num7:
        newChar = "7";
        break;
    case sf::Keyboard::Key::Num8:
        newChar = "8";
        break;
    case sf::Keyboard::Key::Num9:
        newChar = "9";
        break;

    case sf::Keyboard::Key::Numpad0:
        newChar = "0";
        break;
    case sf::Keyboard::Key::Numpad1:
        newChar = "1";
        break;
    case sf::Keyboard::Key::Numpad2:
        newChar = "2";
        break;
    case sf::Keyboard::Key::Numpad3:
        newChar = "3";
        break;
    case sf::Keyboard::Key::Numpad4:
        newChar = "4";
        break;
    case sf::Keyboard::Key::Numpad5:
        newChar = "5";
        break;
    case sf::Keyboard::Key::Numpad6:
        newChar = "6";
        break;
    case sf::Keyboard::Key::Numpad7:
        newChar = "7";
        break;
    case sf::Keyboard::Key::Numpad8:
        newChar = "8";
        break;
    case sf::Keyboard::Key::Numpad9:
        newChar = "9";
        break;

    default:
        newChar = "";
    }

    std::string temp = m_body.getString() + newChar;
    m_body.setString(temp);

    //Delete
    if(e.key.code == sf::Keyboard::BackSpace)
    {
        if(temp.size() > 0)
        {
            temp.erase(temp.size() - 1, 1);
            m_body.setString(temp);
        }
    }

    CreateSelectionVertices();

}
