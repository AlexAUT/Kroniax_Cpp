#ifndef GUILAYER_HPP
#define GUILAYER_HPP

#include <vector>
#include <string>
#include <SFML/System/Vector2.hpp>

#include "../../include/gui/guiBaseClass.hpp"

namespace sf
{
	class Event;
	class RenderWindow;
}

namespace aw
{

	class GuiLayer
	{
	public:
		GuiLayer();


		void addElement(GuiType type, const std::string& ID, const sf::Vector2f& position, const std::string& text);

		void handleEvent(const sf::Event &e);

		void render(sf::RenderWindow &window);

		//
		// Setter
		//

		void setActiveElement(unsigned int index);

		//
		// Getter
		//

		GuiBaseElement* getLastElement();
		GuiBaseElement* getElement(unsigned int index);
		GuiBaseElement* getSelectedElement();

	private:

		std::vector<GuiBaseElement> m_elements;

		unsigned int m_activeElement;

	};

}

#endif // GUILAYER_HPP
