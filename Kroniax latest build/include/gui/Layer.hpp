#ifndef GUILAYER_HPP
#define GUILAYER_HPP

#include <vector>

#include "GuiBaseClass.hpp"

class GuiLayer
{
public:
    GuiLayer();

    ~GuiLayer();


    void AddElement(int type, std::string ID, sf::Vector2f position, std::string text);

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
