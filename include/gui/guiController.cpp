#include "../../include/gui/guiController.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace aw
{

	GuiController::GuiController() : m_activeLayer(0)
	{
		
	}

	void GuiController::loadFont(const std::string &path)
	{
		m_font.loadFromFile(path);
	}

	void GuiController::addLayer()
	{
		m_layers.push_back(GuiLayer());
	}

	void GuiController::addLabel(unsigned int layer, const std::string& ID, const sf::Vector2f& pos, const std::string& text)
	{
		addElement(layer, GUI_LABEL, ID, pos, text);
	}

	void GuiController::addButton(unsigned int layer, const std::string& ID, const sf::Vector2f& pos, const std::string& text)
	{
		addElement(layer, GUI_BUTTON, ID, pos, text);
	}

	void GuiController::addList(unsigned int layer, const std::string& ID, const sf::Vector2f& pos, const std::string& text)
	{
		addElement(layer, GUI_LIST, ID, pos, text);
	}

	void GuiController::addInput(unsigned int layer, const std::string& ID, const sf::Vector2f& pos, const std::string& text)
	{
		addElement(layer, GUI_INPUT, ID, pos, text);
	}

	void GuiController::handleEvent(const sf::Event &e)
	{

		m_layers[m_activeLayer].handleEvent(e);

	}

	void GuiController::render(sf::RenderWindow &window)
	{
		m_layers[m_activeLayer].render(window);
	}

	void GuiController::addElement(unsigned int layer, GuiType type, const std::string& ID, const sf::Vector2f& pos, const std::string& text)
	{
		if (layer < m_layers.size())
		{
			m_layers[layer].addElement(type, ID, pos, text);
			m_layers[layer].getLastElement()->setFont(m_font);
		}
	}


	//
	// Setter
	//

	int GuiController::getActiveLayerInt()
	{
		return m_activeLayer;
	}

	void GuiController::setActiveLayer(unsigned int index)
	{
		if (index < m_layers.size())
		{
			m_activeLayer = index;
			m_layers[m_activeLayer].setActiveElement(0);
		}
	}

	//
	// Getter
	//

	GuiLayer* GuiController::getLastLayer()
	{
		return &m_layers[m_layers.size() - 1];
	}

	const sf::Font &GuiController::getFont() const
	{
		return m_font;
	}

	GuiLayer* GuiController::getLayer(unsigned int index)
	{
		if (index < m_layers.size())
		{
			return &m_layers[index];
		}

		return nullptr;
	}

	GuiLayer* GuiController::getSelectedLayer()
	{
		return &m_layers[m_activeLayer];
	}



	GuiBaseElement* GuiController::getElement(unsigned int indexLayer, unsigned int indexElement)
	{
		if (indexLayer < m_layers.size())
		{
			return m_layers[indexLayer].getElement(indexElement);
		}

		return nullptr;
	}

	GuiBaseElement* GuiController::getSelectedElement()
	{
		return m_layers[m_activeLayer].getSelectedElement();
	}

}