#include "networkHandler.hpp"

#include "../messageBus/messageBus.hpp"

#include <SFML/Network/Packet.hpp>
#include <SFML/Network/IpAddress.hpp>

#include <iostream>

namespace aw
{
	NetworkHandler::NetworkHandler(MessageBus &messageBus) :
		m_messageBus(messageBus)
	{
		m_messageBus.addReceiver(this);
	}

	void NetworkHandler::update()
	{
		if (m_connected)
		{
			sf::Packet receivedData;

			if (m_socket.receive(receivedData) == sf::Socket::Done)
			{
				std::size_t command;
				receivedData >> command;
				if (command == std::hash<std::string>()("game list"))
				{
					Message mapList(std::hash<std::string>()("game list"));
					while (!receivedData.endOfPacket())
					{
						std::string name;
						receivedData >> name;
						mapList.push_back(name);
					}

					m_messageBus.sendMessage(mapList);
				}
				else if (command == std::hash<std::string>()("joined game"))
				{
					Message msg;
					msg.ID = std::hash<std::string>()("start game");
					std::string name;
					receivedData >> name;
					msg.push_back(name);
					msg.push_back(static_cast<std::string>("online time challenge"));
					int gameState;
					receivedData >> gameState;
					msg.push_back(gameState);
					//Time left until the next onlineState change
					float timeLeft;
					receivedData >> timeLeft;
					msg.push_back(timeLeft);

					while (!receivedData.endOfPacket())
					{
						std::string playerName;
						float playerTime;
						receivedData >> playerName >> playerTime;
						msg.push_back(playerName);
						msg.push_back(playerTime);
					}

					m_messageBus.sendMessage(msg);
				}
				else if (command == std::hash<std::string>()("new player"))
				{
					Message msg;
					msg.ID = command; //command = hash("new player")
					std::string name;
					receivedData >> name;
					msg.push_back(name);
					m_messageBus.sendMessage(msg);
				}
				else if (command == std::hash<std::string>()("remove player"))
				{
					Message msg;
					msg.ID = command; //command = hash("remove player")
					std::string name;
					receivedData >> name;
					msg.push_back(name);
					m_messageBus.sendMessage(msg);
				}
				else if (command == std::hash<std::string>()("load map"))
				{
					Message msg;
					msg.ID = std::hash<std::string>()("new map");
					std::string mapName;
					receivedData >> mapName;
					msg.push_back(mapName);
					m_messageBus.sendMessage(msg);
				}
				else if (command == std::hash<std::string>()("mode loading"))
				{
					Message msg;
					msg.ID = std::hash<std::string>()("onlinemode loading");
					float timeToNextAction;
					receivedData >> timeToNextAction;
					msg.push_back(timeToNextAction);
					m_messageBus.sendMessage(msg);
				}
				else if (command == std::hash<std::string>()("mode running"))
				{
					Message msg;
					msg.ID = std::hash<std::string>()("onlinemode running");
					float timeToNextAction;
					receivedData >> timeToNextAction;
					msg.push_back(timeToNextAction);
					m_messageBus.sendMessage(msg);
				}
				else if (command == std::hash<std::string>()("mode finish"))
				{
					Message msg;
					msg.ID = std::hash<std::string>()("onlinemode finish");
					float timeToNextAction;
					receivedData >> timeToNextAction;
					msg.push_back(timeToNextAction);
					m_messageBus.sendMessage(msg);
				}
				else if (command == std::hash<std::string>()("new best time"))
				{
					Message msg;
					msg.ID = std::hash<std::string>()("new best time");
					std::string name;
					receivedData >> name;
					float time;
					receivedData >> time;

					msg.push_back(name);
					msg.push_back(time);

					m_messageBus.sendMessage(msg);
				}
				else if (command == std::hash<std::string>()("ping request"))
				{
					sf::Packet toSend;
					toSend << std::hash<std::string>()("I am alive");
					m_socket.send(toSend);
				}
			}
		}
	}

	void NetworkHandler::receiveMessage(const Message &msg)
	{
		if (msg.ID == std::hash<std::string>()("close game"))
		{
			if (m_connected)
			{
				sf::Packet toSend;
				toSend << std::hash<std::string>()("disconnect");
				//m_socket.send(toSend);
				
				m_connected = false;
				m_socket.disconnect();
			}
		}
		else if (msg.ID == std::hash<std::string>()("connect"))
		{
			std::string name = *msg.getValue<std::string>(0);
			Message answer;
			answer.ID = std::hash<std::string>()("result connecting");
			answer.push_back(connectToMasterServer(name));
			m_messageBus.sendMessage(answer);
		}
		else if (msg.ID == std::hash<std::string>()("request game list"))
		{
			if (m_connected)
			{

				sf::Packet toSend;
				toSend << std::hash<std::string>()("get game list");
				m_socket.send(toSend);
			}
		}
		else if (msg.ID == std::hash<std::string>()("quit online game"))
		{
			sf::Packet toSend;
			toSend << std::hash<std::string>()("quit game");
			m_socket.send(toSend);
		}
		else if (msg.ID == std::hash<std::string>()("join server"))
		{
			sf::Packet toSend;
			toSend << std::hash<std::string>()("join game");
			toSend << *msg.getValue<std::string>(0);

			m_socket.send(toSend);
		}
		else if (msg.ID == std::hash<std::string>()("new time"))
		{
			sf::Packet toSend;
			toSend << std::hash<std::string>()("new time");
			toSend << *msg.getValue<float>(0);
			
			m_socket.send(toSend);
		}
	}


	bool NetworkHandler::connectToMasterServer(const std::string &name)
	{
		m_socket.setBlocking(true);

		sf::Socket::Status status = m_socket.connect("127.0.0.1", 12121, sf::seconds(1));

		if (status != sf::Socket::Done)
		{
			m_connected = false;
			return false;
		}
		
		m_socket.setBlocking(false);
		m_connected = true;

		//Send also the name
		sf::Packet toSend;
		toSend << std::hash<std::string>()("name");
		toSend << name;
		m_socket.send(toSend);
		//Return success
		return true;
	}
}