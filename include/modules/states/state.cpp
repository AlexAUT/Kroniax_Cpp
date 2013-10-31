#include "state.hpp"

#include "../stateMachine.hpp"

namespace aw
{
	State::State(StateMachine &stateMachine) :
        m_stateMachine(stateMachine)
    {}

    State::~State()
    {
    }

	void State::changeActiveState(const std::string &name)
	{
		m_stateMachine.changeActiveState(name);
	}
}
