#ifndef AWSTATEMACHINE_HPP
#define AWSTATEMACHINE_HPP

#include <map>
#include <memory>

#include "states/state.hpp"

//Forward declaration
namespace sf
{
	class Time;
	class RenderWindow;
}

namespace aw
{
	class StateMachine
	{
	public:
		StateMachine();

		void update(const sf::Time &frameTime);
		void render(sf::RenderWindow &window);

		//Carefully! Will take the ownership of the parameter
		void addState(const std::string &name, std::unique_ptr<State> ptr);
		void removeState(const std::string &name);

		void changeActiveState(const std::string &name);

	private:

	private:
		std::map<std::string, std::unique_ptr<State>> m_states;
		State *m_activeState;
	};
}

#endif //AWSTATEMACHINE_HPP