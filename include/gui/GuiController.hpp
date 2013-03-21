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

    void AddElement(unsigned int layer, int type, std::string ID, sf::Vector2f pos, std::string text);

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



private:

    sf::Font m_font;

    std::vector<GuiLayer> m_layers;

    unsigned int m_activeLayer;

};


#endif // GUICONTROLLER_HPP
