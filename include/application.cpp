#include "application.hpp"

#include <iostream>

#include "messageBus\receiver.hpp"

//Include states
#include "modules/states/state.hpp"
#include "modules/states/intro.hpp"
#include "modules/states/menu.hpp"
#include "modules/states/game.hpp"

namespace aw
{

	Application::Application() :
		m_settings(m_messageBus), m_windowHandler(m_messageBus)
	{
		//Init Statemachine
		m_stateMachine.addState("intro", std::unique_ptr<State>(new Intro(m_stateMachine)));
		m_stateMachine.addState("menu", std::unique_ptr<State>(new Menu(m_stateMachine, m_messageBus)));
		m_stateMachine.addState("game", std::unique_ptr<State>(new Game(m_stateMachine, m_messageBus)));
		
		//Set intro as active State
		m_stateMachine.changeActiveState("menu");
	
		//Load settings afterwards, because it will send informations to the states
		//Sound, window settings
		m_settings.load();
	}

	int Application::run()
	{
		while (m_windowHandler.getWindow().isOpen())
		{
			//Update the frameTimer
			m_frameTime = m_frameTimer.restart();

			//Check for Events (Will send them to the MessageBus)
			m_windowHandler.checkEventQueue();

			//Update active State
			m_stateMachine.update(m_frameTime);

			//Clear the rendertarget
			m_windowHandler.getWindow().clear();

			//Draw active State
			m_stateMachine.render(m_windowHandler.getWindow());

			//Display frame
			m_windowHandler.getWindow().display();
		}

		m_settings.save();

		return 0;
	}

}