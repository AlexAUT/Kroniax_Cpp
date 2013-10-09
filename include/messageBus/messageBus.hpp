#ifndef MESSAGEBUS_HPP
#define MESSAGEBUS_HPP
/////INCLUDES/////
#include <vector>

//////FORWARD DECLARATION/////
namespace aw
{
	class Receiver;
	struct Message;
}

namespace aw
{
	class MessageBus
	{
	public:
		void addReceiver(Receiver *newReceiver);
		void removeReceiver(Receiver *receiver);
		void sendMessage(Message &msg);
	private:

	private:
		std::vector<Receiver*> m_receivers;
	};
}

#endif