#ifndef GUIBASECLASS_HPP
#define GUIBASECLASS_HPP

#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace sf
{
	class RenderWindow;
	class Font;
	class Event;
}


enum GuiType
{
    GUI_LABEL,
    GUI_BUTTON,
    GUI_LIST,
    GUI_INPUT
};

class GuiBaseElement
{
public:
    GuiBaseElement();
    GuiBaseElement(GuiType type, const std::string& ID, const sf::Vector2f& m_position, const std::string& text);


    void HandleEvents(sf::Event &e);

    void Draw(sf::RenderWindow &window);

    void AddEntry(std::string entry);
    void ClearEntries();

    //
    // Getter
    //

    const std::string& GetID();
    const sf::Vector2f& GetPosition();
    std::string GetText();
    sf::Text &GetTextObj();
    bool IsSelectable() const;

    //
    // Setter
    //


    void SetID(std::string ID);
    void SetSelected(bool value);
    void SetSelectable(bool value);
    void SetFont(sf::Font &font);
    void SetPosition(sf::Vector2f position);
    void SetX(float x);
    void SetY(float y);
    void SetText(std::string text);
    void SetActiveEntry(unsigned int index);
    void SetCharacterSize(unsigned int size);


    void HandleKeyInput(sf::Event &e);

protected:

    virtual void CreateSelectionVertices();

private:

    sf::Text m_body;


private:
    GuiType m_type;

    bool m_selected;
    bool m_selectable;

    std::string m_ID;

    sf::Vector2f m_position;

    sf::Vertex m_vertices [6];


    std::vector<std::string> m_entries;
    int m_activeEntry;


};

#endif // GUIBASECLASS_HPP
