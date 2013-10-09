#ifndef AWSTATE_HPP
#define AWSTATE_HPP

#include <string>

//Forward declaration
namespace sf
{
	class Time;
	class RenderWindow;
}

namespace aw
{
	class StateMachine;

	class State
	{
	public:
		State(StateMachine &stateMachine);

		virtual void update(const sf::Time &frameTime) = 0;
		virtual void render(sf::RenderWindow &window) = 0;

	protected:
		void changeActiveState(const std::string &name);

	private:
		StateMachine &m_stateMachine;

	};
}

#endif