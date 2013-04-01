#ifndef GUIBASECLASS_HPP
#define GUIBASECLASS_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace sf
{
	class RenderWindow;
	class Font;
	class Event;
}

#include <string>

class GuiBaseElement
{
public:

    GuiBaseElement();
    GuiBaseElement(int type, std::string ID, sf::Vector2f m_position, std::string text);


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
    bool GetSelectAble();

    //
    // Setter
    //


    void SetID(std::string ID);
    void SetSelected(bool value);
    void SetSelectAble(bool value);
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

    int m_type; // 1 = button , 2 = list

    bool m_selected;
    bool m_selectAble;

    std::string m_ID;

    sf::Vector2f m_position;

    sf::Vertex m_vertices [6];


    std::vector<std::string> m_entries;
    int m_activeEntry;


};

#endif // GUIBASECLASS_HPP
