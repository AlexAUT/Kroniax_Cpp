#include "../../include/gui/guiLayer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace aw
{

	GuiLayer::GuiLayer() : m_activeElement(0)
	{

	}

	void GuiLayer::addElement(GuiType type, const std::string& ID, const sf::Vector2f& position, const std::string& text)
	{
		m_elements.push_back(GuiBaseElement(type, ID, position, text));
	}


	void GuiLayer::handleEvent(const sf::Event &e)
	{
		m_elements[m_activeElement].handleEvent(e);

		if (e.type == sf::Event::KeyPressed)
		{
			if (e.key.code == sf::Keyboard::Down)
			{
				m_elements[m_activeElement].setSelected(false);


				if (m_activeElement == m_elements.size() - 1)
				{
					m_activeElement = 0;
				}
				else
				{
					m_activeElement++;
				}


				while (!m_elements[m_activeElement].isSelectable())
				{
					if (m_activeElement == m_elements.size() - 1)
					{
						m_activeElement = 0;
					}
					else
					{
						m_activeElement++;
					}
				}

				m_elements[m_activeElement].setSelected(true);

			}

			if (e.key.code == sf::Keyboard::Up)
			{
				m_elements[m_activeElement].setSelected(false);

				if (m_activeElement == 0)
				{
					m_activeElement = m_elements.size() - 1;
				}
				else
				{
					m_activeElement--;
				}


				while (!m_elements[m_activeElement].isSelectable())
				{
					if (m_activeElement == 0)
					{
						m_activeElement = m_elements.size() - 1;
					}
					else
					{
						m_activeElement--;
					}
				}

				m_elements[m_activeElement].setSelected(true);

			}
		}
	}

	void GuiLayer::render(sf::RenderWindow &window)
	{
		if (m_activeElement < m_elements.size())
			m_elements[m_activeElement].setSelected(true);


		for (unsigned int i = 0; i < m_elements.size(); i++)
		{
			m_elements[i].render(window);
		}
	}

	//
	// Setter
	//

	void GuiLayer::setActiveElement(unsigned int index)
	{


		if (index < m_elements.size())
		{
			for (unsigned int i = 0; i < m_elements.size(); i++)
			{
				m_elements[i].setSelected(false);
			}

			m_elements[index].setSelected(true);
			m_activeElement = index;

			return;
		}
	}


	//
	// Getter
	//

	GuiBaseElement* GuiLayer::getLastElement()
	{
		return &m_elements[m_elements.size() - 1];
	}

	GuiBaseElement* GuiLayer::getElement(unsigned int index)
	{
		if (index < m_elements.size())
		{
			return &m_elements[index];
		}

		return nullptr;
	}

	GuiBaseElement* GuiLayer::getSelectedElement()
	{
		return &m_elements[m_activeElement];
	}

}