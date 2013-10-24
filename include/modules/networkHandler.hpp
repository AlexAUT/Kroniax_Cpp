#ifndef NETWORKHANDLER_HPP
#define NETWORKHANDLER_HPP

#include "../messageBus/receiver.hpp"

#include <SFML/Network/TcpSocket.hpp>

namespace aw
{
	class MessageBus;

	class NetworkHandler : public Receiver
	{
	public:

		NetworkHandler(MessageBus &messageBus);

		//Look for received data
		void update();

		void receiveMessage(const Message &msg);

	private:

		bool connectToMasterServer(const std::string &name);

	private:
		MessageBus &m_messageBus;

		bool m_connected;

		sf::TcpSocket m_socket;
	};
}

#endif