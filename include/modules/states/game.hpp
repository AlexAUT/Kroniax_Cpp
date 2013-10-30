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
#include "../game/timeTable.hpp"
#include "../game/gameTimer.hpp"
#include "../game/countdown.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/Audio/Music.hpp>

#include <vector>

namespace aw
{
	enum class GameType
	{
		OFFICIAL_ARCADE,
		OFFICIAL_TIMECHALLENGE,
		ONLINE_TIME_CHALLENGE
	};

	enum class GameState
	{
		STOPPED,
		CRASHED,
		RUNNING,
		FINISHED,
		PAUSED,
		CHAT,
		HELP
	};

	enum OnlineState
	{
		LOADING,
		RUNNING,
		FINISHED
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

		void initMusic();

		void loadLevel();
		void loadPlayerInformation(const std::string &path);

		void resetToLastCheckpoint();
		void resetToStart();

		//Send the message for completing a level to unlcoka new one and save the progress
		void sendInformationLevelFinished(bool startNextLevel);
		//Check the keyboard to control the [0] player
		void playerControl();
		//Send information that player has started this ship
		void sendStartInformation();
		//Informs the server that the player crashed/paused/finished = PlayerState::Stopped
		void sendStopInformation();
		//Inform the server that you have quit a online game...
		void quitOnlineGame();

		std::vector<Player>::iterator searchPlayer(const std::string &name);

	private:
		bool m_active; //Important. Is needed to turn of the gui after for example starting a game

		MessageBus &m_messageBus;

		bool m_ignoreNextKeyEvent;

		std::string m_levelName;
		GameType m_gameType;
		GameState m_gameState;
		GameState m_gameStateBeforeChatting;
		OnlineState m_onlineState;

		MapRenderer m_mapRenderer;
		CollisionSystem m_collisionSystem;
		std::vector<Player> m_players;
		Camera m_camera;
		ScriptManager m_scriptManager;
		TimeTable m_timeTable;
		GameTimer m_gameTimer;
		Countdown m_countDownNextAction;

		GuiController m_gui;//Will display different game screens
		sf::Clock m_startingTimer;

		//Background music
		sf::Music m_music;
		int m_openMusic;
	};
}

#endif //AWGAME_HPP