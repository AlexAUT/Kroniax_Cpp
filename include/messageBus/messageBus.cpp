#include "messageBus.hpp"

#include "receiver.hpp"

//#define NDEBUG
#include <assert.h>

namespace aw
{
	void MessageBus::addReceiver(Receiver *newReceiver)
	{
		//Error check. 1Receiver 2times in the vector makes no sense
		#ifndef NDEBUG
		for (auto *it : m_receivers)
		{
				assert(newReceiver != it);
		}
		#endif

		m_receivers.push_back(newReceiver);
	}

	void MessageBus::removeReceiver(Receiver *receiver)
	{
		for (auto it = m_receivers.begin(); it != m_receivers.end(); ++it)
		{
			if (*it == receiver)
			{
				m_receivers.erase(it);
				//1 receiver can't be 2times in the vector so escape the loop
				break;
			}
		}
	}

	void MessageBus::sendMessage(Message &msg)
	{
		for (auto *it : m_receivers)
		{
			it->receiveMessage(msg);
		}
	}
}