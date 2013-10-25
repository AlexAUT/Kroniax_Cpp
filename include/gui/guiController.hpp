#ifndef GUICONTROLLER_HPP
#define GUICONTROLLER_HPP

#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Font.hpp>

#include "../../include/gui/guiLayer.hpp"

namespace sf
{
	class Event;
	class RenderWindow;
}

namespace aw
{

	class GuiBaseElement;

	class GuiController
	{
	public:
		GuiController();

		void loadFont(const std::string &path);

		void addLayer();

		void addLabel(unsigned int layer, const std::string& ID, const sf::Vector2f& pos, const std::string& text);
		void addButton(unsigned int layer, const std::string& ID, const sf::Vector2f& pos, const std::string& text);
		void addList(unsigned int layer, const std::string& ID, const sf::Vector2f& pos, const std::string& text);
		void addInput(unsigned int layer, const std::string& ID, const sf::Vector2f& pos, const std::string& text);

		void handleEvent(const sf::Event &e);

		void render(sf::RenderWindow &window);

		//
		// Setter
		//

		void setActiveLayer(unsigned int index);

		//
		// Getter
		//
		int getActiveLayerInt();

		const sf::Font &getFont() const;

		GuiLayer* getLastLayer();
		GuiLayer* getLayer(unsigned int index);
		GuiLayer* getSelectedLayer();

		GuiBaseElement* getElement(unsigned int indexLayer, unsigned int indexElement);
		GuiBaseElement* getSelectedElement();


	private:

		void addElement(unsigned int layer, GuiType type, const std::string& ID, const sf::Vector2f& pos, const std::string& text);

		sf::Font m_font;

		std::vector<GuiLayer> m_layers;

		unsigned int m_activeLayer;

	};

}

#endif // GUICONTROLLER_HPP
