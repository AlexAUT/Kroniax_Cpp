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

namespace aw
{

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


		void handleEvent(const sf::Event &e);

		void render(sf::RenderWindow &window) const;

		void addEntry(std::string entry);
		void clearEntries();

		//
		// Getter
		//

		const std::string& getID() const;
		const sf::Vector2f& getPosition() const;
		std::string getText() const;
		const sf::Text& getTextObj() const;
		bool isSelectable() const;

		//
		// Setter
		//


		void setID(std::string ID);
		void setSelected(bool value);
		void setSelectable(bool value);
		void setFont(const sf::Font& font);
		void setPosition(sf::Vector2f position);
		void setX(float x);
		void setY(float y);
		void setText(std::string text);
		void setActiveEntry(unsigned int index);
		void setCharacterSize(unsigned int size);

		void setObjectToCenter(int windowWidth);

	protected:

		virtual void createSelectionVertices();

	private:

		sf::Text m_body;

		GuiType m_type;

		bool m_selected;
		bool m_selectable;

		std::string m_ID;

		sf::Vector2f m_position;

		sf::Vertex m_vertices[6];

		sf::Color m_selectColor;


		std::vector<std::string> m_entries;
		int m_activeEntry;
	};
}
#endif // GUIBASECLASS_HPP
