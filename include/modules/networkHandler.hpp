#ifndef NETWORKHANDLER_HPP
#define NETWORKHANDLER_HPP

#include "../messageBus/receiver.hpp"

#include <SFML/Network/TcpSocket.hpp>

#include <string>

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
		std::string m_name;

		sf::TcpSocket m_socket;
	};
}

#endif
