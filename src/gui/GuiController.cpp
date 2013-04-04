#include "../../include/gui/GuiController.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

GuiController::GuiController() : m_activeLayer(0)
{
    m_font.loadFromFile("data/font/good times.ttf");
}

GuiController::~GuiController()
{

}

void GuiController::AddLayer()
{
    m_layers.push_back(GuiLayer());
}

void GuiController::AddLabel(unsigned int layer, const std::string& ID, const sf::Vector2f& pos, const std::string& text)
{
    AddElement(layer, GUI_LABEL, ID, pos, text);
}

void GuiController::AddButton(unsigned int layer, const std::string& ID, const sf::Vector2f& pos, const std::string& text)
{
    AddElement(layer, GUI_BUTTON, ID, pos, text);
}

void GuiController::AddList(unsigned int layer, const std::string& ID, const sf::Vector2f& pos, const std::string& text)
{
    AddElement(layer, GUI_LIST, ID, pos, text);
}

void GuiController::AddInput(unsigned int layer, const std::string& ID, const sf::Vector2f& pos, const std::string& text)
{
    AddElement(layer, GUI_INPUT, ID, pos, text);
}

void GuiController::AddElement(unsigned int layer, GuiType type, const std::string& ID, const sf::Vector2f& pos, const std::string& text)
{
    if(layer < m_layers.size())
    {
        m_layers[layer].AddElement(type, ID, pos, text);
        m_layers[layer].GetLastElement()->SetFont(m_font);
    }
}

void GuiController::HandleEvent(const sf::Event &e)
{

    m_layers[m_activeLayer].HandleEvent(e);

}

void GuiController::Draw(sf::RenderWindow &window)
{
    m_layers[m_activeLayer].Draw(window);
}


//
// Setter
//

int GuiController::GetActiveLayerInt()
{
    return m_activeLayer;
}

void GuiController::SetActiveLayer(unsigned int index)
{
    if(index < m_layers.size())
    {
        m_activeLayer = index;
        m_layers[m_activeLayer].SetActiveElement(0);
    }
}

//
// Getter
//

GuiLayer* GuiController::GetLastLayer()
{
    return &m_layers[m_layers.size()-1];
}

GuiLayer* GuiController::GetLayer(unsigned int index)
{
    if(index < m_layers.size())
    {
        return &m_layers[index];
    }

    return nullptr;
}

GuiLayer* GuiController::GetSelectedLayer()
{
    return &m_layers[m_activeLayer];
}



GuiBaseElement* GuiController::GetElement(unsigned int indexLayer, unsigned int indexElement)
{
    if(indexLayer < m_layers.size())
    {
        return m_layers[indexLayer].GetElement(indexElement);
    }

    return nullptr;
}

GuiBaseElement* GuiController::GetSelectedElement()
{
    return m_layers[m_activeLayer].GetSelectedElement();
}
