#ifndef AWGAME_HPP
#define AWGAME_HPP

#include "state.hpp"
#include "../../messageBus/receiver.hpp"
#include "../../gui/guiController.hpp"

#include "../game/mapRenderer.hpp"
#include "../game/player.hpp" 
#include "../game/camera.hpp"
#include "../game/collisionSystem.hpp"
#include "../game/scriptManager.hpp"

#include <SFML/System/Clock.hpp>


namespace aw
{
	enum class GameType
	{
		OFFICIAL_ARCADE,
		OFFICIAL_TIMECHALLENGE,
		ARCADE,
		TIMECHALLENGE
	};

	enum class GameState
	{
		STOPPED,
		PAUSED,
		STARTING,
		RUNNING
	};

	/////FORWARD DECL/////
	class MessageBus;
	/////CLASS DECL//////
	class Game : public State, public Receiver
	{
	public:
		Game(StateMachine &statemachine, MessageBus &messageBus);
	
		void update(const sf::Time &frameTime);
		void render(sf::RenderWindow &window);

		void receiveMessage(const Message &msg);

	private:

		void loadLevel();
		void loadPlayerInformation(const std::string &path);

		void resetToLastCheckpoint();

	private:
		bool m_active; //Important. Is needed to turn of the gui after for example starting a game

		MessageBus &m_messageBus;

		std::string m_levelName;
		GameType m_gameType;
		GameState m_gameState;

		MapRenderer m_mapRenderer;
		CollisionSystem m_collisionSystem;
		Player m_player;
		Camera m_camera;
		ScriptManager m_scriptManager;

		GuiController m_gui;//Will display different game screens
		sf::Clock m_startingTimer;
	};
}

#endif //AWGAME_HPP