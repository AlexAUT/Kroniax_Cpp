#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "messageBus\messageBus.hpp"
#include "modules\settings.hpp"
#include "modules/windowHandler.hpp"
#include "modules/stateMachine.hpp"

#include <SFML/System/Clock.hpp>

namespace aw
{
	class Application
	{
	public:

		Application();

		int run();

	private:

	private:
		MessageBus m_messageBus;
		//Modules//////////////////////
		Settings m_settings;
		WindowHandler m_windowHandler;
		StateMachine m_stateMachine;
		///////////////////////////////
		sf::Clock m_frameTimer;
		sf::Time m_frameTime;

	};
}

#endif