#ifndef AWGAME_HPP
#define AWGAME_HPP

#include "state.hpp"
#include "../../messageBus/receiver.hpp"

#include "../game/mapRenderer.hpp"
#include "../game/player.hpp" 
#include "../game/camera.hpp"
#include "../game/collisionSystem.hpp"
#include "../game/scriptManager.hpp"

namespace aw
{
	enum class GameType
	{
		OFFICIAL_ARCADE,
		OFFICIAL_TIMECHALLENGE,
		ARCADE,
		TIMECHALLENGE
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

	private:
		bool m_active; //Important. Is needed to turn of the gui after for example starting a game

		MessageBus &m_messageBus;

		std::string m_levelName;
		GameType m_gameType;

		MapRenderer m_mapRenderer;
		CollisionSystem m_collisionSystem;
		Player m_player;
		Camera m_camera;
		ScriptManager m_scriptManager;
		
	};
}

#endif //AWGAME_HPP