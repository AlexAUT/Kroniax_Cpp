#ifndef GUILAYER_HPP
#define GUILAYER_HPP

#include "../../include/gui/GuiBaseClass.hpp"

#include <SFML/System/Vector2.hpp>

namespace sf
{
	class Event;
	class RenderWindow;
}

#include <vector>
#include <string>

class GuiLayer
{
public:
    GuiLayer();

    ~GuiLayer();


    void AddElement(GuiType type, const std::string& ID, const sf::Vector2f& position, const std::string& text);

    void HandleEvents(sf::Event &e);

    void Draw(sf::RenderWindow &window);

    //
    // Setter
    //

    void SetActiveElement(unsigned int index);

    //
    // Getter
    //

    GuiBaseElement* GetLastElement();
    GuiBaseElement* GetElement(unsigned int index);
    GuiBaseElement* GetSelectedElement();

private:

private:

    std::vector<GuiBaseElement> m_elements;

    unsigned int m_activeElement;

};


#endif // GUILAYER_HPP
