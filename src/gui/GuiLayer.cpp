#include "../../include/gui/GuiLayer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

GuiLayer::GuiLayer() : m_activeElement(0)
{

}

GuiLayer::~GuiLayer()
{

}

void GuiLayer::AddElement(GuiType type, const std::string& ID, const sf::Vector2f& position, const std::string& text)
{
    m_elements.push_back(GuiBaseElement(type, ID, position, text));
}


void GuiLayer::HandleEvents(sf::Event &e)
{
    m_elements[m_activeElement].HandleEvents(e);

    if(e.type == sf::Event::KeyPressed)
    {
        if(e.key.code == sf::Keyboard::Down)
        {
            m_elements[m_activeElement].SetSelected(false);


            if(m_activeElement == m_elements.size()-1)
            {
                m_activeElement = 0;
            }
            else
            {
                m_activeElement++;
            }


            while(!m_elements[m_activeElement].IsSelectable())
            {
                if(m_activeElement == m_elements.size()-1)
                {
                    m_activeElement = 0;
                }
                else
                {
                    m_activeElement++;
                }
            }

            m_elements[m_activeElement].SetSelected(true);

        }

        if(e.key.code == sf::Keyboard::Up)
        {
            m_elements[m_activeElement].SetSelected(false);

            if(m_activeElement == 0)
            {
                m_activeElement = m_elements.size()-1;
            }
            else
            {
                m_activeElement--;
            }


            while(!m_elements[m_activeElement].IsSelectable())
            {
                if(m_activeElement == 0)
                {
                    m_activeElement = m_elements.size()-1;
                }
                else
                {
                    m_activeElement--;
                }
            }

            m_elements[m_activeElement].SetSelected(true);

        }
    }
}

void GuiLayer::Draw(sf::RenderWindow &window)
{
    if(m_activeElement < m_elements.size())
        m_elements[m_activeElement].SetSelected(true);


    for(unsigned int i = 0; i < m_elements.size(); i++)
    {
        m_elements[i].Draw(window);
    }
}

//
// Setter
//

void GuiLayer::SetActiveElement(unsigned int index)
{


    if(index < m_elements.size())
    {
        for(unsigned int i = 0; i < m_elements.size(); i++)
        {
            m_elements[i].SetSelected(false);
        }

        m_elements[index].SetSelected(true);
        m_activeElement = index;

        return;
    }
}


//
// Getter
//

GuiBaseElement* GuiLayer::GetLastElement()
{
    return &m_elements[m_elements.size()-1];
}

GuiBaseElement* GuiLayer::GetElement(unsigned int index)
{
    if(index < m_elements.size())
    {
        return &m_elements[index];
    }

    return nullptr;
}

GuiBaseElement* GuiLayer::GetSelectedElement()
{
    return &m_elements[m_activeElement];
}
