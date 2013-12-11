#ifndef AWMENU_HPP
#define AWMENU_HPP

#include "state.hpp"

#include "../../gui/guiController.hpp"
#include "../game/mapRenderer.hpp"

#include "../../messageBus/receiver.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/View.hpp>

#include <SFML/Audio/Music.hpp>

#include <vector>

namespace aw
{
	class MessageBus;

	class Menu : public State, public Receiver
	{
	public:
		Menu(StateMachine &stateMachine, MessageBus &messageBus);

		void update(const sf::Time &frameTime);
		void render(sf::RenderWindow &window);

		void receiveMessage(const Message &msg);

	private:

		void initGui();
		void resetView();

		void loadRandomLevel();

		void updateLevelInformation();
		void updateGameInformation(const sf::Time &frameTime);

		//Gui action events (when a button is triggered etc.)
		void buttonAction();
		void mainLayer();
		void arcadeLayer();
		void tutorial1Layer();
		void tutorial2Layer();
		void optionsLayer();
		void creditsLayer();
		void setUpConnectionLayer();
		void mapSelectionLayer();
		void connectionFailedLayer();
		void helpMultiplayerLayer();


	private:
		bool m_active; //Important to turn of the receive message function in some cases

		MessageBus &m_messageBus;
		//This message will be send after reading the mutliplayer information
		Message m_messageMutliplayerStart;

		GuiController m_gui;
		MapRenderer m_mapRenderer;
		sf::RectangleShape m_overlay; //Will overlay the mapRenderer

		sf::Texture m_texLogo;
		sf::Sprite  m_logo;

		sf::View m_view;

		sf::Music m_music;

		unsigned int m_unlockedLevels;

		struct LevelInformation
		{
			std::string name, author, difficulty, lentgh = "";
		} m_levelInformation;

		struct GameInformation
		{
			std::string name;
			std::string mapName;
			unsigned int playerNumber;

			GameInformation(const std::string &pName, const std::string &pMapName, unsigned int pPlayerNumber) :
				name(pName), mapName(pMapName), playerNumber(pPlayerNumber)
			{}
		};
		std::vector<GameInformation> m_gameList; //Will be used to store the online serverlist
		sf::Time m_timeSinceUpdateGameList;
	};
}

#endif