#include "stateMachine.hpp"

#include "states/state.hpp"

#include <assert.h>

namespace aw
{
	StateMachine::StateMachine() :
		m_activeState(nullptr)
	{}

	void StateMachine::update(const sf::Time &frameTime)
	{
		if (m_activeState)
			m_activeState->update(frameTime);
	}

	void StateMachine::render(sf::RenderWindow &window)
	{
		if (m_activeState)
			m_activeState->render(window);
	}

	void StateMachine::addState(const std::string &name, std::unique_ptr<State> &ptr)
	{
		m_states[name] = std::move(ptr);
	}

	void StateMachine::removeState(const std::string &name)
	{
		auto it = m_states.find(name);

		if (it != m_states.end())
		{
			m_states.erase(it);
		}
	}

	void StateMachine::changeActiveState(const std::string &name)
	{
		auto it = m_states.find(name);

		assert(it != m_states.end());

		m_activeState = &*it->second;
	}
}