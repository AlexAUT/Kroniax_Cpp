#include "networkHandler.hpp"

#include "../messageBus/messageBus.hpp"
#include "../utilities/hash.hpp"

#include <SFML/System/Vector2.hpp>

#include <SFML/Network/Packet.hpp>
#include <SFML/Network/IpAddress.hpp>

#include <iostream>

namespace aw
{
	NetworkHandler::NetworkHandler(MessageBus &messageBus) :
        m_messageBus(messageBus),
        m_connected(false)
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
				unsigned int command;
				receivedData >> command;

				//First check for key states updates ->they are called frequently
				if (command == aw::hash("space pressed"))
				{
					std::string name;
					receivedData >> name;

					sf::Vector2f position;
					sf::Vector2f speed;
					float gravitation = 0;

					if (!receivedData.endOfPacket())
					{
						receivedData >> position.x >> position.y;
						receivedData >> speed.x >> speed.y;
						receivedData >> gravitation;
					}

					Message msg;
					msg.ID = command;
					msg.push_back(name);
					msg.push_back(position);
					msg.push_back(speed);
					msg.push_back(gravitation);

					m_messageBus.sendMessage(msg);
				}
				else if (command == aw::hash("space released"))
				{
					std::string name;
					receivedData >> name;

					sf::Vector2f position;
					sf::Vector2f speed;
					float gravitation = 0;

					if (!receivedData.endOfPacket())
					{
						receivedData >> position.x >> position.y;
						receivedData >> speed.x >> speed.y;
						receivedData >> gravitation;
					}

					Message msg;
					msg.ID = command;
					msg.push_back(name);
					msg.push_back(position);
					msg.push_back(speed);
					msg.push_back(gravitation);

					m_messageBus.sendMessage(msg);
				}
				else if (command == aw::hash("left pressed"))
				{
					std::string name;
					receivedData >> name;

					sf::Vector2f position;
					sf::Vector2f speed;
					float gravitation = 0;

					if (!receivedData.endOfPacket())
					{
						receivedData >> position.x >> position.y;
						receivedData >> speed.x >> speed.y;
						receivedData >> gravitation;
					}

					Message msg;
					msg.ID = command;
					msg.push_back(name);
					msg.push_back(position);
					msg.push_back(speed);
					msg.push_back(gravitation);

					m_messageBus.sendMessage(msg);
				}
				else if (command == aw::hash("left released"))
				{
					std::string name;
					receivedData >> name;

					sf::Vector2f position;
					sf::Vector2f speed;
					float gravitation = 0;

					if (!receivedData.endOfPacket())
					{
						receivedData >> position.x >> position.y;
						receivedData >> speed.x >> speed.y;
						receivedData >> gravitation;
					}

					Message msg;
					msg.ID = command;
					msg.push_back(name);
					msg.push_back(position);
					msg.push_back(speed);
					msg.push_back(gravitation);

					m_messageBus.sendMessage(msg);
				}
				else if (command == aw::hash("right pressed"))
				{
					std::string name;
					receivedData >> name;

					sf::Vector2f position;
					sf::Vector2f speed;
					float gravitation = 0;

					if (!receivedData.endOfPacket())
					{
						receivedData >> position.x >> position.y;
						receivedData >> speed.x >> speed.y;
						receivedData >> gravitation;
					}

					Message msg;
					msg.ID = command;
					msg.push_back(name);
					msg.push_back(position);
					msg.push_back(speed);
					msg.push_back(gravitation);

					m_messageBus.sendMessage(msg);
				}
				else if (command == aw::hash("right released"))
				{
					std::string name;
					receivedData >> name;

					sf::Vector2f position;
					sf::Vector2f speed;
					float gravitation = 0;

					if (!receivedData.endOfPacket())
					{
						receivedData >> position.x >> position.y;
						receivedData >> speed.x >> speed.y;
						receivedData >> gravitation;
					}

					Message msg;
					msg.ID = command;
					msg.push_back(name);
					msg.push_back(position);
					msg.push_back(speed);
					msg.push_back(gravitation);

					m_messageBus.sendMessage(msg);
				}
				// Other tasks
				else if (command == aw::hash("start online try"))
				{
					std::string name;
					sf::Vector2f position;
					sf::Vector2f speed;
					float gravitation;

					receivedData >> name;
					receivedData >> position.x >> position.y;
					receivedData >> speed.x >> speed.y;
					receivedData >> gravitation;

					Message toSend;
					toSend.ID = command;
					toSend.push_back(name);
					toSend.push_back(position);
					toSend.push_back(speed);
					toSend.push_back(gravitation);

					m_messageBus.sendMessage(toSend);
				}
				else if (command == aw::hash("stop online try"))
				{
					Message msg;
					msg.ID = command;

					std::string name;
					receivedData >> name;
					msg.push_back(name);

					m_messageBus.sendMessage(msg);
				}
				else if (command == aw::hash("game list"))
				{
					Message mapList(aw::hash("game list"));
					while (!receivedData.endOfPacket())
					{
						std::string name;
						receivedData >> name;
						mapList.push_back(name);
					}

					m_messageBus.sendMessage(mapList);
				}
				else if (command == aw::hash("joined game"))
				{
					Message msg;
					msg.ID = aw::hash("start game");
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
					msg.push_back(m_name);

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
				else if (command == aw::hash("new player"))
				{
					//std::cout << "\a\a";
					Message msg;
					msg.ID = command; //command = hash("new player")
					std::string name;
					receivedData >> name;
					msg.push_back(name);
					m_messageBus.sendMessage(msg);
				}
				else if (command == aw::hash("remove player"))
				{
					Message msg;
					msg.ID = command; //command = hash("remove player")
					std::string name;
					receivedData >> name;
					msg.push_back(name);
					m_messageBus.sendMessage(msg);
				}
				else if (command == aw::hash("load map"))
				{
					Message msg;
					msg.ID = aw::hash("new map");
					std::string mapName;
					receivedData >> mapName;
					msg.push_back(mapName);
					m_messageBus.sendMessage(msg);
				}
				else if (command == aw::hash("mode loading"))
				{
					Message msg;
					msg.ID = aw::hash("onlinemode loading");
					float timeToNextAction;
					receivedData >> timeToNextAction;
					msg.push_back(timeToNextAction);
					m_messageBus.sendMessage(msg);
				}
				else if (command == aw::hash("mode running"))
				{
					Message msg;
					msg.ID = aw::hash("onlinemode running");
					float timeToNextAction;
					receivedData >> timeToNextAction;
					msg.push_back(timeToNextAction);
					m_messageBus.sendMessage(msg);
				}
				else if (command == aw::hash("mode finish"))
				{
					Message msg;
					msg.ID = aw::hash("onlinemode finish");
					float timeToNextAction;
					receivedData >> timeToNextAction;
					msg.push_back(timeToNextAction);
					m_messageBus.sendMessage(msg);
				}
				else if (command == aw::hash("new best time"))
				{
					Message msg;
					msg.ID = aw::hash("new best time");
					std::string name;
					receivedData >> name;
					float time;
					receivedData >> time;

					msg.push_back(name);
					msg.push_back(time);

					m_messageBus.sendMessage(msg);
				}
				else if (command == aw::hash("global ladder"))
				{
					Message msg;
					msg.ID = aw::hash("global ladder");
					std::string name;
					float time;
					while (!receivedData.endOfPacket())
					{
						receivedData >> name >> time;
						msg.push_back(name);
						msg.push_back(time);
					}
					m_messageBus.sendMessage(msg);
				}
				else if (command == aw::hash("ping request"))
				{
					sf::Packet toSend;
					toSend << aw::hash("I am alive");
					m_socket.send(toSend);
				}
			}
		}
	}

	void NetworkHandler::receiveMessage(const Message &msg)
	{
		//Place the key states first because they will be send frequently
		if (msg.ID == aw::hash("p space pressed"))
		{
			sf::Packet toSend;
			toSend << aw::hash("space pressed");
			toSend << msg.getValue<sf::Vector2f>(0)->x << msg.getValue<sf::Vector2f>(0)->y;
			toSend << msg.getValue<sf::Vector2f>(1)->x << msg.getValue<sf::Vector2f>(1)->y;
			toSend << *msg.getValue<float>(2);
			m_socket.send(toSend);
		}
		else if (msg.ID == aw::hash("p space released"))
		{
			sf::Packet toSend;
			toSend << aw::hash("space released");
			toSend << msg.getValue<sf::Vector2f>(0)->x << msg.getValue<sf::Vector2f>(0)->y;
			toSend << msg.getValue<sf::Vector2f>(1)->x << msg.getValue<sf::Vector2f>(1)->y;
			toSend << *msg.getValue<float>(2);
			m_socket.send(toSend);
		}
		else if (msg.ID == aw::hash("p left pressed"))
		{
			sf::Packet toSend;
			toSend << aw::hash("left pressed");
			toSend << msg.getValue<sf::Vector2f>(0)->x << msg.getValue<sf::Vector2f>(0)->y;
			toSend << msg.getValue<sf::Vector2f>(1)->x << msg.getValue<sf::Vector2f>(1)->y;
			toSend << *msg.getValue<float>(2);
			m_socket.send(toSend);
		}
		else if (msg.ID == aw::hash("p left released"))
		{
			sf::Packet toSend;
			toSend << aw::hash("left released");
			toSend << msg.getValue<sf::Vector2f>(0)->x << msg.getValue<sf::Vector2f>(0)->y;
			toSend << msg.getValue<sf::Vector2f>(1)->x << msg.getValue<sf::Vector2f>(1)->y;
			toSend << *msg.getValue<float>(2);
			m_socket.send(toSend);
		}
		else if (msg.ID == aw::hash("p right pressed"))
		{
			sf::Packet toSend;
			toSend << aw::hash("right pressed");
			toSend << msg.getValue<sf::Vector2f>(0)->x << msg.getValue<sf::Vector2f>(0)->y;
			toSend << msg.getValue<sf::Vector2f>(1)->x << msg.getValue<sf::Vector2f>(1)->y;
			toSend << *msg.getValue<float>(2);
			m_socket.send(toSend);
		}
		else if (msg.ID == aw::hash("p right released"))
		{
			sf::Packet toSend;
			toSend << aw::hash("right released");
			toSend << msg.getValue<sf::Vector2f>(0)->x << msg.getValue<sf::Vector2f>(0)->y;
			toSend << msg.getValue<sf::Vector2f>(1)->x << msg.getValue<sf::Vector2f>(1)->y;
			toSend << *msg.getValue<float>(2);
			m_socket.send(toSend);
		}
		else if (msg.ID == aw::hash("p start online try"))
		{
			sf::Packet toSend;
			toSend << aw::hash("start online try");
			toSend << msg.getValue<sf::Vector2f>(0)->x;
			toSend << msg.getValue<sf::Vector2f>(0)->y;
			toSend << msg.getValue<sf::Vector2f>(1)->x;
			toSend << msg.getValue<sf::Vector2f>(1)->y;
			toSend << *msg.getValue<float>(2);

			m_socket.send(toSend);
		}
		else if (msg.ID == aw::hash("p stop online try"))
		{
			sf::Packet toSend;
			toSend << aw::hash("stop online try");

			m_socket.send(toSend);
		}
		else if (msg.ID == aw::hash("close game"))
		{
			if (m_connected)
			{
				sf::Packet toSend;
				toSend << aw::hash("disconnect");
				m_socket.send(toSend);
				
				m_connected = false;
				m_socket.disconnect();
			}
		}
		else if (msg.ID == aw::hash("connect"))
		{
			std::string name = *msg.getValue<std::string>(0);
			m_name = name;
			Message answer;
			answer.ID = aw::hash("result connecting");
			answer.push_back(connectToMasterServer(name));
			m_messageBus.sendMessage(answer);
		}
		else if (msg.ID == aw::hash("request game list"))
		{
			if (m_connected)
			{
				sf::Packet toSend;
				toSend << aw::hash("get game list");
				m_socket.send(toSend);
			}
		}
		else if (msg.ID == aw::hash("quit online game"))
		{
			sf::Packet toSend;
			toSend << aw::hash("quit game");
			m_socket.send(toSend);
		}
		else if (msg.ID == aw::hash("join server"))
		{
			sf::Packet toSend;
			toSend << aw::hash("join game");
			toSend << *msg.getValue<std::string>(0);

			m_socket.send(toSend);
		}
		else if (msg.ID == aw::hash("new time"))
		{
			sf::Packet toSend;
			toSend << aw::hash("new time");
			toSend << *msg.getValue<float>(0);
			
			m_socket.send(toSend);
		}
	}


	bool NetworkHandler::connectToMasterServer(const std::string &name)
	{
		m_socket.setBlocking(true);

		//sf::Socket::Status status = m_socket.connect("82.211.56.205", 12121, sf::seconds(1));
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
		toSend << aw::hash("name");
		toSend << name;
		m_socket.send(toSend);
		//Return success
		return true;
	}
}
