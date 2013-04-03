#ifndef GUICONTROLLER_HPP
#define GUICONTROLLER_HPP

#include "../../include/gui/GuiLayer.hpp"

class GuiBaseElement;

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Font.hpp>

namespace sf
{
	class Event;
	class RenderWindow;
}

#include <string>
#include <vector>

class GuiController
{
public:
    GuiController();
    ~GuiController();

    void AddLayer();

    void AddLabel(unsigned int layer, const std::string& ID, const sf::Vector2f& pos, const std::string& text);
    void AddButton(unsigned int layer, const std::string& ID, const sf::Vector2f& pos, const std::string& text);
    void AddList(unsigned int layer, const std::string& ID, const sf::Vector2f& pos, const std::string& text);
    void AddInput(unsigned int layer, const std::string& ID, const sf::Vector2f& pos, const std::string& text);

    void HandleEvents(sf::Event &e);

    void Draw(sf::RenderWindow &window);

    //
    // Setter
    //

    void SetActiveLayer(unsigned int index);

    //
    // Geter
    //
    int GetActiveLayerInt();

    GuiLayer* GetLastLayer();
    GuiLayer* GetLayer(unsigned int index);
    GuiLayer* GetSelectedLayer();

    GuiBaseElement* GetElement(unsigned int indexLayer, unsigned int indexElement);
    GuiBaseElement* GetSelectedElement();


private:

    void AddElement(unsigned int layer, GuiType type, const std::string& ID, const sf::Vector2f& pos, const std::string& text);

    sf::Font m_font;

    std::vector<GuiLayer> m_layers;

    unsigned int m_activeLayer;

};


#endif // GUICONTROLLER_HPP
