#include "state.hpp"

#include "../stateMachine.hpp"

namespace aw
{
	State::State(StateMachine &stateMachine) :
		m_stateMachine(stateMachine)
	{}

	void State::changeActiveState(const std::string &name)
	{
		m_stateMachine.changeActiveState(name);
	}
}