#ifndef AWWINDOWHANDLER_HPP
#define AWWINDOWHANDLER_HPP

#include "../messageBus/receiver.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

/////FORWARD DECLARATION/////
namespace aw
{
	class MessageBus;
}

namespace aw
{
	class WindowHandler : public Receiver
	{
	public:
		WindowHandler(MessageBus &msgBus);

		void checkEventQueue();

		void receiveMessage(const Message &msg);

		sf::RenderWindow &getWindow();

	private:

	private:
		MessageBus &m_messageBus;

		sf::RenderWindow m_window;

	};
}

#endif