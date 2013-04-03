#ifndef GUILAYER_HPP
#define GUILAYER_HPP

#include <vector>
#include <string>
#include <SFML/System/Vector2.hpp>

#include "../../include/gui/GuiBaseClass.hpp"

namespace sf
{
	class Event;
	class RenderWindow;
}


class GuiLayer
{
public:
    GuiLayer();

    ~GuiLayer();


    void AddElement(GuiType type, const std::string& ID, const sf::Vector2f& position, const std::string& text);

    void HandleEvent(const sf::Event &e);

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

    std::vector<GuiBaseElement> m_elements;

    unsigned int m_activeElement;

};


#endif // GUILAYER_HPP
