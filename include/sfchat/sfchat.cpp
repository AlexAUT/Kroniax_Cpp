////////////////////////////////////////////////////////////
//
// sfchat - a class for providing in-game chat between players connected to a networked game built on SFML.
// Copyright (C) 2013 Matt Marchant
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

//source code for the sfchat networked in-game chat class

#include "sfchat.h"

#include <SFML/Network/Packet.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/Network/SocketSelector.hpp>

using namespace sfchat;

///////////////////////////////////////////////////////////////////////
// Configuration varibles. Use these along with variables in sfchat.h
// to configure behaviour of chat class.
///////////////////////////////////////////////////////////////////////
//key to press to enable text input - change to whichever you like
const sf::Keyboard::Key Chat::m_chatEnableKey = sf::Keyboard::T;
//key to press to disable / cancel text input
const sf::Keyboard::Key Chat::m_chatDisableKey = sf::Keyboard::Escape;
//maximum number of chars allowed in input buffer string
const sf::Uint8 Chat::m_maxBufferLength = 75u; //change to Uint16/32 if needing more than 255
//maximum number of chat lines to be buffered. Older lines are discarded when new lines are added beyond
//this value. All lines are kept in log file when logging is enabled.
const sf::Uint8 Chat::m_maxChatLines = 20u;
//prefix for input buffer. This is removed from buffer when line is sent
const std::string Chat::m_inputBufferPrefix = "Say: ";
//maximum number of clients allowed to connect to server
const sf::Uint8 Chat::m_maxClients = 15u; //16 users including server
//maximum length of username
const sf::Uint8 Chat::m_maxNickLength = 30u;
//number of seconds to try connecting before timing out
const float Chat::m_connectTimeout = 10.f;
//IMPORTANT// Change this to a value unique to your project. This is a basic attempt
//at preventing unwanted clients joining your server by abusing this source code (it does happen).
//This value is sent by the client upon connect and the server will kick clients which do not
//match. Not 100% secure but better than nothing.
const std::string Chat::m_authString = "de587fje883h3vejr9fjwb9fjwdhc8";


///////////////////////////////////////////////////////////////////////
// Class Definitions
///////////////////////////////////////////////////////////////////////


///***operator overloads for sf::Packet***///
sf::Packet& operator << (sf::Packet& packet, const NetData nd)
{
	return packet << nd.name << nd.message << nd.team << nd.uid << nd.nickList;
}

sf::Packet& operator >> (sf::Packet& packet, NetData& nd)
{
	return packet >> nd.name >> nd.message >> nd.team >> nd.uid >> nd.nickList;
}


///***Chat class***///
//ctor
Chat::Chat()
    :
	m_outputString(""), //uhm, what?   
	m_username("ChatUser"),
    m_uid(0u),
    m_currentTeam(TEAM_ALL),
    m_motd("Welcome to sfchat! Commands available are: /nick, /me, /team, /connect, /disconnect and /play"),
    m_allowInput(false),
    m_lastKey(sf::Keyboard::Unknown),
    m_currentState(NotConnected),
    m_loggingEnabled(false), //this value is overriden by Create and Connect, regardless
    m_timestampEnabled(true),
    m_port(1000u),
    m_localNameChange(false),
    m_sendToTeam(false),
    m_runThread(false),
    m_localTime(0)
{
#ifdef SEED_RAND
	srand((unsigned)time(0));
#endif
	m_RandName(m_username);
}

//dtor
Chat::~Chat()
{
	//tidily kill any running threads
	m_KillThread();

	//close any open log files
	EnableLogging(false);
}

//***public functions***//
void Chat::Update(sf::Event& event)
{
	//poll event for keyboard input
	if(m_allowInput) //set to allow input by pressing m_chatKeyFirst
	{
		//check for special cases
		if(event.type == sf::Event::KeyPressed)
		{
			switch(event.key.code)
			{
			case sf::Keyboard::Return:
				if(m_lastKey != sf::Keyboard::Return
					&& m_inputBuffer.size() > m_inputBufferPrefix.size()) //don't send 'empty' strings
				{
					//process the input buffer
					m_ProcessInput();
					
					//disable input ready for next input
					m_allowInput = true;
					m_inputBuffer = m_inputBufferPrefix;
					m_lastKey = m_chatEnableKey;

					//log keypress
					m_lastKey = sf::Keyboard::Return;
				}
				return;
			case sf::Keyboard::BackSpace:
				//delete last char
				if(m_inputBuffer.size() > m_inputBufferPrefix.size())//dont try and delete when buffer 'empty'
					m_inputBuffer.pop_back();
				return;
			case m_chatDisableKey: 
				if(m_lastKey != m_chatDisableKey)
				{
					//cancel input
					m_allowInput = false;
					m_lastKey = m_chatDisableKey;
				}
				return;
			default: break;
			}
		}
		//reset last key on release
		else if(event.type == sf::Event::KeyReleased
			&& event.key.code == m_lastKey)
		{
			m_lastKey = sf::Keyboard::Unknown;
			return;
		}
		//else append new char
		else if (event.type == sf::Event::TextEntered
			&& m_lastKey == sf::Keyboard::Unknown)
		{
			// Handle ASCII characters only, skipping backspace and delete
			if(event.text.unicode > 31
				&& event.text.unicode < 127
				&& m_inputBuffer.size() < m_maxBufferLength)
			{
				m_inputBuffer += static_cast<char>(event.text.unicode);
			}
		}
	}
	else //check to see if we are enabling input
	{
		if(event.type == sf::Event::KeyPressed
			&& event.key.code == m_chatEnableKey)
		{
				m_allowInput = true;
				m_inputBuffer = m_inputBufferPrefix;
				m_lastKey = m_chatEnableKey;
		}
		//use this for testing chat logging
		
		/*if(event.type == sf::Event::KeyPressed
			&& event.key.code == sf::Keyboard::Y)
			EnableLogging(!m_loggingEnabled);*/
		
	}
}

void Chat::Create(sf::Uint16 port, bool log)
{
	//check we aren't already serving or connected elsewhere
	if(m_currentState != NotConnected)
	{
		std::string state = (m_currentState == ChatServer) ? "Server" : "Client" ;
		std::cout << "Chat " << state << "is already running. Please disconnect first." << std::endl;
		return;
	}

	//we should never have any running threads here so we don't need to lock
	m_port = port;
	m_loggingEnabled = log;

	m_timeNow = time(0);
	m_localTime = localtime(&m_timeNow);
	char buffer[20];
	strftime(buffer, 20, "%x %X", m_localTime);
	std::string str = buffer;
	m_OutputString("Started server: " + str);

	//start the listen server in own thread
	m_netThread = std::unique_ptr<sf::Thread>(new sf::Thread(&Chat::m_Server, this));
	m_currentState = ChatServer;
	m_runThread = true;
	m_netThread->launch();

	//if console input is enabled start input thread
#ifdef ENABLE_CONSOLE
	m_consoleThread = std::unique_ptr<sf::Thread>(new sf::Thread(&Chat::m_ConsoleInput, this));
	m_consoleThread->launch();
#endif
}

void Chat::Connect(sf::IpAddress address, sf::Uint16 port, bool log)
{
	//check we aren't already serving or connected elsewhere
	if(m_currentState != NotConnected)
	{
		std::string state = (m_currentState == ChatServer) ? "Server" : "Client" ;
		std::cout << "Chat " << state << "is already running. Please disconnect first." << std::endl;
		return;
	}

	m_destIp = address;
	m_port = port;
	m_loggingEnabled = log;

	//notify window of connection attempt
	m_OutputString("Connecting to: " + m_destIp.toString());
#ifdef DEBUG_TEXT
	std::cout << "Connecting to: " << m_destIp.toString() << std::endl;
#endif

	//start the client in own thread
	m_netThread = std::unique_ptr<sf::Thread>(new sf::Thread(&Chat::m_Client, this));
	m_currentState = ChatClient;
	m_runThread = true;
	m_netThread->launch();

	//if console input is enabled start input thread
#ifdef ENABLE_CONSOLE
	m_consoleThread = std::unique_ptr<sf::Thread>(new sf::Thread(&Chat::m_ConsoleInput, this));
	m_consoleThread->launch();
#endif
}

void Chat::Disconnect(void)
{
	m_KillThread();
	m_OutputString("Disconnected.");
	m_nickList.clear();
	m_uid = 0u;
}

const std::string Chat::GetOutputText()
{
	//lock this so other threads don't attempt to update it mid-output
	sf::Lock lock(m_mutex);
	std::string str = m_outputString;

	return str;
}

const std::string Chat::GetInputText()
{
	if(m_allowInput) //only return something to display if waiting input
	{
		//add a blinking cursor
		std::string retVal = m_inputBuffer;
		if(m_cursorClock.getElapsedTime().asSeconds() > 0.5f)
		{
			retVal += "|";
			if(m_cursorClock.getElapsedTime().asSeconds() > 1.f)
				m_cursorClock.restart();
		}
		
		return retVal;
	}
	else
		return "";
}

const std::string Chat::GetNickList()
{
	sf::Lock locak(m_mutex);
	std::string str = m_nickList;
	
	return str;
}

const std::deque<TextLine>& Chat::GetOutputBuffer(void)
{
	return m_outputBuffer;
}

void Chat::EnableLogging(bool b)
{
	m_mutex.lock();
	m_loggingEnabled = b;
	m_mutex.unlock();

	if(m_loggingEnabled)
	{
		//open file stream for writing
		m_logFile.open("chatlog.txt", std::ios::app);
		if(m_logFile.good())
		{
			m_timeNow = time(0);
			m_localTime = localtime(&m_timeNow);
			//opened the file for logging so add a date to say where we started
			char buffer[40];
			strftime(buffer, 40, "Log started on: %x %X", m_localTime);
			m_logFile << std::endl << buffer << std::endl;

#ifdef DEBUG_TEXT
			std::cout << "Logging Enabled" << std::endl;
#endif
		}
	}
	else
	{
		if(m_logFile.is_open())
		{
			m_logFile << std::endl << "-----------Log Session Ends-----------" << std::endl;
			m_logFile.close();
#ifdef DEBUG_TEXT
			std::cout << "Logging Disabled" << std::endl;
#endif
		}
	}
}

void Chat::EnableTimeStamp(bool b)
{
	m_timestampEnabled = b;
}

void Chat::EnableInput()
{
	m_allowInput = true;
	m_inputBuffer = m_inputBufferPrefix;
	m_lastKey = sf::Keyboard::Unknown;
}

bool Chat::WaitingInput(void)
{
	return m_allowInput;
}

const ChatState Chat::GetCurrentState(void)
{
	return m_currentState;
}

void Chat::SetUserName(std::string name)
{
	sf::Lock lock(m_mutex);
	m_username = name.substr(0, m_maxNickLength);
}

void Chat::SetClientTeam(sf::Uint8 team)
{
	if(m_currentState != ChatServer)
		m_currentTeam = team;
}

void Chat::SetMotd(std::string string)
{
	m_motd = string;
}

void Chat::ExecIrcCmd(std::string cmd)
{
	if(cmd.front() != '/') return; //not a command
	
	//temp stash the buffer if there is anything on it
	std::string temp;
	if(!m_inputBuffer.empty())
		temp = m_inputBuffer;
	//parse the command
	sf::Lock lock(m_mutex);
	m_inputBuffer = m_inputBufferPrefix + cmd;
	m_ProcessInput();

	//return the buffer if needs be
	m_inputBuffer = temp;
}

//***private functions***//
void Chat::m_OutputString(std::string str, sf::Color colour)
{	
	//check string is valid
	if(str.empty()) return;

	//log chat and timestamp if enabled
	if(m_loggingEnabled)
	{
		if(m_logFile.is_open())
		{
			if(m_timestampEnabled)
			{
				m_timeNow = time(0);
				m_localTime = localtime(&m_timeNow);

				char buffer[10];
				strftime(buffer, 10, "%X", m_localTime);
				m_logFile << buffer << " " << str << std::endl;
			}
			else
			{
				m_logFile << str << std::endl;
			}
		}
	}
	m_outputBuffer.push_back(TextLine(str, colour));
	//remove oldest line if length greater than max buffer size
	if(m_outputBuffer.size() > m_maxChatLines)
		m_outputBuffer.pop_front();

	//update output string as buffer content has changed
	m_UpdateOutput();

	//echo copy to console if enabled
#ifdef ENABLE_CONSOLE
	std::cout << str << std::endl;
#endif
}

void Chat::m_UpdateOutput()
{
	//must lock when updating from another thread
	//to prevent trying to return an incomplete string via GetString methods
	sf::Lock lock(m_mutex);
	m_outputString.clear();
	for(auto i = m_outputBuffer.cbegin(); i != m_outputBuffer.cend(); ++i)
		m_outputString += i->text + "\n";
	
}

void Chat::m_KillThread(void)
{
	m_runThread = false;	
	//kill any threads running network client / server	
	if(m_netThread)
	{
		m_netThread->wait(); //IMPORTANT must make sure thread will actually end as this is blocking
		m_netThread.reset();
	}
	//kill therad managing console input
	if(m_consoleThread)
	{
		m_consoleThread->wait();
		m_consoleThread.reset();
	}

	m_currentState = NotConnected;
}

std::string Chat::m_ParseCommand(std::string str)
{
	//executes IRC style commands if command is known
	//individual commands can be enabled or disabled
	//at the top of sfchat.h by setting the const vals to 0
	
	//pluck command from beginning of string
	std::string cmd = str.substr(0, str.find_first_of(' '));

	//let's do something with the command
	if(ENABLE_ME && cmd == "/me") //perform an action
	{
		if(str.size() < cmd.size() + 1) return "";
		return m_username + str.substr(str.find_first_of(' '));
	}
	else if(ENABLE_NICK && cmd == "/nick") //change client username
	{
		if(str.size() < cmd.size() + 1) return "";
		
		//TODO check new name not same as old name
		std::string oldname = m_username;
		SetUserName(str.substr(str.find_first_of(' ') + 1));
		if(m_currentState == ChatClient) m_localNameChange = true;
		return oldname + " is now known as " + m_username;
	}
	else if(ENABLE_TEAM && cmd == "/team")
	{
		if(str.size() < cmd.size() + 1) return "";
		
		if(m_currentState != ChatServer) //want to send to all clients anyway
		{
			sf::Lock lock(m_mutex);
			m_sendToTeam = true;
		}
		return "<" + m_username + ">: (team) " + str.substr(str.find_first_of(' ') + 1);
	}
	else if(ENABLE_CONNECT && cmd == "/connect")
	{
		//disconnect if already connected
		if(m_currentState != NotConnected)
			Disconnect();
		Connect(sf::IpAddress(str.substr(str.find_first_of(' ') + 1)));
		return "silent";
	}
	else if(ENABLE_DISCONNECT && cmd == "/disconnect")
	{
		Disconnect();
		return "Bye Bye!";
	}
	else if(ENABLE_KICK && cmd == "/kick")
	{
		if(str.size() < cmd.size() + 1) return "";
		if(m_currentState != ChatServer)
		{
			//only server can kick clients
			m_OutputString("You do not have sufficient privileges.", COLOUR_SERVER_MESSAGE);
		} 
		else
		{
			sf::Lock lock(m_mutex);
			m_kickRequest = str.substr(str.find_first_of(' ') + 1);
		}
		return "silent";
	}
	else
		return ""; //command not recognised
}

void Chat::m_RandName(std::string& name)
{
	char buffer[5];
	sprintf(buffer, "%i", rand() % 9999);
	name += buffer;
}

void Chat::m_ProcessInput(void)
{
	std::string sendStr;
	{
		//crop prefix
		sf::Lock lock(m_mutex);
		sendStr = m_inputBuffer.substr(m_inputBufferPrefix.size(), m_inputBuffer.size());
	}
	if(sendStr.front() == '/')
	{
		//try to parse command
		sendStr = m_ParseCommand(sendStr);
		if(sendStr.empty()) //returned an Unknown command
		{
			if(m_currentState != ChatServer) //don't print if server else message will be sent to all clients
				m_OutputString("Unknown Command");
		}
		else if(sendStr == "silent")
			return; //command doesn't want us to print anything
	}
	else
		//prepend player name to string
		sendStr = "<" + m_username + ">: " + sendStr;

	//send text
	if(m_currentState != ChatClient) //add string to output buffer if local / server
	{
		m_OutputString(sendStr);
		sf::Lock lock(m_mutex);
		m_uid = 0; //make sure to state we're sensing a normal message
	}
	else
	{
		//send client string directly to server (the server will send the line back to us when it updates all clients)
		sf::Lock lock(m_mutex);
		m_clientBuffer = sendStr;
	}
}

void Chat::m_UpdateNickList(std::list< std::shared_ptr<Client> >& clients)
{
	//update the nick list
	sf::Lock lock(m_mutex);
	m_nickList.clear();
	for(auto it = clients.begin(); it != clients.end(); ++it)
	{
		Client& client = **it;
		m_nickList += client.name + "\n";
	}
}

void Chat::m_PlaySound(std::string sound)
{
#ifdef DEBUG_TEXT
	std::cout << "Played sound " << sound << std::endl;
#endif

	//TODO define your own version of this function here to hook your audio engine
}

//***threaded functions***//
void Chat::m_Server(void)
{
	// Create a socket to listen to new connections
	sf::TcpListener listener;
	listener.listen(m_port);

	// Create a list to store the future clients
	std::list< std::shared_ptr<Client> > clients;

	// Create a selector
	sf::SocketSelector selector;

	// Add the listener to the selector
	selector.add(listener);

	//change the username
	m_username = "Server";

#ifdef DEBUG_TEXT
	std::cout << "Starting chat server on port: " << m_port << std::endl;
#endif
	//set the logging to default value from create function
	EnableLogging(m_loggingEnabled);

	// Enter loop to handle new connection and updating of server / clients
	while(m_runThread)
	{		
		//Make the selector wait for data on any socket
		if(selector.wait(sf::Time(sf::seconds(0.5f))))
		{
			//If there are available slots test the listener - TODO report server full to client
			if(selector.isReady(listener))
			{
				//The listener is ready: there is a pending connection
				std::shared_ptr<Client> client = std::shared_ptr<Client>(new Client);
				
				if(listener.accept(*client->socket) == sf::Socket::Done)
				{
					sf::Packet packet;
					if(clients.size() < m_maxClients)
					{
						//Add the new client to the clients list
						clients.push_back(client);

						//Add the new client to the selector so that we will
						//be notified when he sends something
						selector.add(*client->socket);

						//stash the end of the current buffer in the client so we don't send it to them
						client->skipMessage = m_outputBuffer.back().text;

						//send motd to new client
						NetData nd("", m_motd, TEAM_ALL, UID_MOTD);
						{
							sf::Lock lock(m_mutex);
							packet << nd;
						}
						client->socket->send(packet);
#ifdef DEBUG_TEXT
						std::cout << "Sent MOTD to client" << std::endl;
#endif
					}
					else
					{
						//send rejection message because server is full and tidy up
						NetData nd("", "Connection refused. Reason: Server full.", TEAM_ALL, client->uid);
#ifdef DEBUG_TEXT
						std::cout << "Refuse connection from " << client->socket->getRemoteAddress() << ". Server full" << std::endl;
#endif
						{
							sf::Lock lock(m_mutex);
							packet << nd;
						}
						client->socket->send(packet);
						client->socket->disconnect();
					}

				}
			}		
			else
			{
				//The listener socket is not ready, test all other sockets (the clients)
				for(auto it = clients.begin(); it != clients.end(); ++it)
				{
					Client& client = **it;
					sf::Packet packet;

					//check to see if we can receive data
					if (selector.isReady(*client.socket))
					{
						//The client has sent some data, we can receive it
						if (client.socket->receive(packet) == sf::Socket::Done)
						{
	                        //parse messages
							NetData nd;
							packet >> nd;
							
							//check to see if name is valid and update as necessary
							if(nd.name != client.name)
							{
								//check name is valid first
								std::string newName = nd.name;
								std::string oldName = client.name;
								if(newName == m_username) //same as server name
									m_RandName(newName);
								else //check all clients
								{
									for(auto it2 = clients.begin(); it2 != clients.end(); ++it2)
									{
										Client& c = **it2;
										//check we aren't comparing with ourself
										if(c.uid != client.uid 
											&& newName == c.name)
										{
											m_RandName(newName);
											break;
										} 
									}
								}
								client.name = newName;
								if(!oldName.empty())
								{
									//m_OutputString(oldName + " changed name to " + newName);
#ifdef DEBUG_TEXT
									std::cout << "Name changed by client" << std::endl;
#endif
								}
								//update the nick list
								m_UpdateNickList(clients);
							}

							//connecting clients do not yet have a uid
							if(!nd.uid)
							{
								if(nd.message == m_authString)
								{
									//notify chat window a client has connected
									sf::Lock lock(m_mutex);
									m_OutputString(nd.name + " (" + client.socket->getRemoteAddress().toString() + ") " "connected.");
									m_UpdateNickList(clients);
								}
								else
								{
									//attempted connection with invalid auth string
									sf::Lock lock(m_mutex);
									m_OutputString(nd.name + " (" + client.socket->getRemoteAddress().toString() + ") " "failed authentication.");

									client.socket->disconnect();
								}
							}
							else if(nd.uid != client.uid)
							{
								//we have a request from the client for the server to do something
								switch(nd.uid)
								{
								case UID_SOUND:
									m_mutex.lock();
									m_soundRequest = nd.message;
									m_uid = UID_ACTION; //TODO stop this getting reset until been sent
									m_mutex.unlock();
									m_OutputString(nd.name + " played " + nd.message);
									break;
								case UID_TEAM:
									m_OutputString(nd.message);
									m_mutex.lock();
									m_uid = UID_TEAM;
									m_mutex.unlock();
									break;
								default: break;
								}
							}
							else if(m_outputBuffer.back().text != nd.message)
							{
								m_OutputString(nd.message);
								if(nd.message.substr(0, nd.name.size()) == nd.name) //we have an action
								{
									sf::Lock lock(m_mutex);
									m_uid = UID_ACTION;
								}
								else
								{
									sf::Lock lock(m_mutex);
									m_uid = 0;
								}
							}
							//check which team message is addressed to and update the team property
							m_currentTeam = nd.team;
						}
					}
				}
			}
		}

		//update all clients
		bool doneUpdate = false;
		unsigned currentSize = clients.size();
		for(auto it = clients.begin(); it != clients.end(); ++it)
		{
			Client& client = **it;

			//check the current message isn't an old message on the buffer
			if(client.skipMessage == m_outputBuffer.back().text)
				continue;
			else client.skipMessage.clear();

			sf::Packet packet;
			
			{
				//send the last string from buffer deque
				sf::Lock lock(m_mutex);
				//we send the client name here, not the server name so clients update if necessary
				//as well as the team the message is intended for
				NetData nd(client.name, m_outputBuffer.back().text, m_currentTeam, client.uid, m_nickList);
				//update the uid if special message
				nd.uid = (m_uid) ? m_uid : client.uid;

				if(m_kickRequest == client.name)
				{
					//server wants to kick this client
					nd.message = "Because I can.";
					nd.uid = UID_KICK;
					m_kickRequest.clear();
				}
				else if(!m_soundRequest.empty())
				{
					//send play request to clients
					nd.message = m_soundRequest;
					nd.uid = UID_SOUND;
				}
				packet << nd;
			}
			sf::Socket::Status result = client.socket->send(packet);

			switch(result)
			{
			case sf::Socket::Done:
#ifdef DEBUG_TEXT
				//std::cout << "Sent: " + m_outputBuffer.back() << std::endl;
#endif
				break;
			case sf::Socket::Disconnected:

				//remove client from selector
				selector.remove(*client.socket);

				//notify when client disconnects
#ifdef DEBUG_TEXT
				std::cout << client.name << " disconnected" << std::endl;
#endif
				m_OutputString(client.name + " has disconnected");
				{
					sf::Lock lock(m_mutex);
					m_uid = UID_SERVER_MESSAGE;
				}

				//remove client from list else we keep sending to non-existent clients
				clients.remove(*it);
				if(clients.size())
					it = clients.begin(); //resetting to beginning is preferable as no clients get skipped
				else doneUpdate = true; //iterator is no longer valid

				break;
			default: break;
			}
			if(doneUpdate) break; //quit loop as we removed all clients
		}
		if(currentSize != clients.size()) //someone disconnected
			m_UpdateNickList(clients);
		//we've looped over all clients so clear any sound requests
		m_soundRequest.clear();
		//and message IDs
		m_uid = 0;
	}
#ifdef ENABLE_CONSOLE
	std::cout << "Press Enter or Return to quit" << std::endl;
#endif
}

void Chat::m_Client(void)
{
	//attempt to connect to server on IP / port
	sf::TcpSocket socket;

	if(socket.connect(m_destIp, m_port, sf::Time(sf::seconds(m_connectTimeout))) == sf::Socket::Done)
	{
#ifdef DEBUG_TEXT
		std::cout << "Successfully started chat client thread." << std::endl;
#endif					
		//send a packet with our name and authstring so server can validate us first
		sf::Packet authPacket;
		NetData authData(m_username, m_authString);
		{
			sf::Lock lock(m_mutex);
			authPacket << authData;
		}
		//TODO check for outcome of this because we can't reasonably continue if we fail to auth
		socket.send(authPacket);

		//set logging
		EnableLogging(m_loggingEnabled);

		//enter into loop to send / receive updates
		while(m_runThread)
		{
			sf::Packet packet;
			sf::Socket::Status result;
			//send message if we have any
			if(!m_clientBuffer.empty())
			{
				sf::Uint8 team = TEAM_ALL;
				sf::Uint32 uid = m_uid;				

				//check if this is a team-only message
				if(m_sendToTeam)
				{
					team = m_currentTeam;
					uid = UID_TEAM;
				}
				//build and send packet
				sf::Lock lock(m_mutex);				
				m_sendToTeam = false;
				NetData nd(m_username, m_clientBuffer, team, uid);
				packet << nd;
				result = socket.send(packet);
				switch(result)
				{
					case sf::Socket::Done:
#ifdef DEBUG_TEXT
						std::cout << "Sent: " << m_clientBuffer << std::endl;
#endif
						//clear the client buffer
						m_clientBuffer.clear();
					break;
					case sf::Socket::Disconnected:
						break; //this is handled by disconnect on receive
					case sf::Socket::Error:
						//TODO report error - optionally disconnect on multiple errors
						break;
					default: break;
				}
			}
			else if(!m_soundRequest.empty())//send sound request to server
			{
				sf::Lock lock(m_mutex);
				NetData nd(m_username, m_soundRequest, TEAM_ALL, UID_SOUND);
				packet << nd;
				socket.send(packet);
				m_soundRequest.clear();
			}

			//check incoming packets
			NetData nd;
			result = socket.receive(packet);
			switch(result)
			{
			case sf::Socket::Done:				
				packet >> nd;
#ifdef DEBUG_TEXT
				//std::cout << "Received: " << nd.message << std::endl;
#endif
				//if message is new send to output buffer
                if(m_outputBuffer.empty() || (nd.message != m_outputBuffer.back().text
                    && nd.uid == m_uid)) //and message is not a server message
				{
					sf::Lock lock(m_mutex);
					m_OutputString(nd.message);
					if(!nd.nickList.empty()) m_nickList = nd.nickList;
				}

				//if server has updated our name
				if(!nd.name.empty() && nd.name != m_username)
				{
					//ignore the server if it tried to revert
					if(m_localNameChange)
					{
						sf::Lock lock(m_mutex);
						m_localNameChange = false;
					}
					else
					{
						std::string oldName = m_username;
						sf::Lock lock(m_mutex);
						m_username = nd.name;
						//send notification to server
						m_clientBuffer = oldName + " changed name to " + m_username;
					}
#ifdef DEBUG_TEXT
					std::cout << "Name changed by server" << std::endl;
#endif
				}
				//we get a UID assigned, or a messsage from the server
				if(nd.uid != m_uid)
				{
					switch(nd.uid)
					{
					case UID_KICK:
						//server requests the client be kicked
						m_mutex.lock();
						m_OutputString("You were kicked from the server. Reason: " + nd.message, COLOUR_SERVER_MESSAGE);
						m_runThread = false;
						m_currentState = NotConnected;
						m_uid = 0u; //must reset our uid so we know we are disconnected
						m_mutex.unlock();
#ifdef DEBUG_TEXT
						std::cout << "You were kicked from the server. Reason: " << nd.message << std::endl;
#endif
						break;
					//case UID_BAN:
					//	break;
					case UID_MOTD:
						m_OutputString(nd.message, COLOUR_SERVER_MESSAGE);
						break;
					case UID_SOUND:
						m_PlaySound(nd.message);
						break;
					case UID_TEAM:
						if(nd.team == m_currentTeam && m_outputBuffer.back().text != nd.message)
							m_OutputString(nd.message, COLOUR_TEAM);
						break;
					case UID_ACTION:
						if(m_outputBuffer.back().text != nd.message)
							m_OutputString(nd.message, COLOUR_ACTION);
						break;
					case UID_SERVER_MESSAGE:
						if(m_outputBuffer.back().text != nd.message)
							m_OutputString(nd.message, COLOUR_SERVER_MESSAGE);
						break;
					default:
						//assign unlisted UID to client if client has no UID yet
						if(!m_uid)
						{
							m_mutex.lock();
							m_uid = nd.uid;
							m_mutex.unlock();
#ifdef DEBUG_TEXT
							std::cout << "Server changed Unique ID to " << nd.uid << std::endl;
#endif
						}
						break;
					}
				}
				break;
			case sf::Socket::Disconnected: //tidy up thread and disconnect gracefully
#ifdef DEBUG_TEXT
				std::cout << "Lost connection to server" << std::endl;
#endif
				m_OutputString("Lost Connection to Server", COLOUR_SERVER_MESSAGE);				
				m_mutex.lock();
				m_runThread = false;
				m_currentState = NotConnected;
				m_mutex.unlock();
				break;
			default: break; //TODO handle socket errors
			}
		}
		socket.disconnect();
		m_nickList.clear();
	}
	else
	{
		//quit with warning if time out
#ifdef DEBUG_TEXT
		std::cout << "Connection attempt timed out" << std::endl;
#endif
		//reset flags
		sf::Lock lock(m_mutex);
		m_OutputString("Connection attempt timed out", COLOUR_SERVER_MESSAGE);
		m_runThread = false;
		m_currentState = NotConnected;
	}
#ifdef ENABLE_CONSOLE
	std::cout << "Press Enter or Return to quit" << std::endl;
#endif
}

void Chat::m_ConsoleInput(void)
{
	//clear anything that might be on the cin buffer
	//TODO check if there is something to actually ignore
	std::cin.ignore();

	while(m_runThread)
	{
		std::string input;
		std::getline(std::cin, input, '\n');
		//TODO check input buffer is clear?
		sf::Lock lock(m_mutex);
		m_inputBuffer = m_inputBufferPrefix + input;
		//process the buffer
		if(m_runThread) m_ProcessInput();
	}
}

