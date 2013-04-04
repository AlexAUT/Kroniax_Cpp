#ifndef GUICONTROLLER_HPP
#define GUICONTROLLER_HPP

#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Font.hpp>

#include "../../include/gui/GuiLayer.hpp"

namespace sf
{
	class Event;
	class RenderWindow;
}

class GuiBaseElement;

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

    void HandleEvent(const sf::Event &e);

    void Draw(sf::RenderWindow &window);

    //
    // Setter
    //

    void SetActiveLayer(unsigned int index);

    //
    // Getter
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
