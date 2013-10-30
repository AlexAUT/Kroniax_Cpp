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

//Header file for SFML in-game network chat class//

#ifndef SFML_CHAT_H_
#define SFML_CHAT_H_

//change this to enable debug text output to console
#define DEBUG_TEXT
//change this to enable chat input / output via console
//not recommended to use this when using an SFML UI
//as it may cause some confusing behaviour
//#define ENABLE_CONSOLE 
//if you seed the default randomiser elsewhere in your program then comment this out
#define SEED_RAND

#include <memory>
#include <iostream>
#include <string>
#include <deque>
#include <list>
//needed for timestamping
#include <time.h>
//needed for text logging
#include <fstream>
//SFML includes
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Clock.hpp>


namespace sfchat
{	
	//////////////////////////////////////////////////////////////////////
	// Configuration variables. Use these to alter behavious of chat class
	// More configuration variables are listed at the top of sfchat.cpp
	//////////////////////////////////////////////////////////////////////
	//default port of server / client
	const sf::Uint16 DEFAULT_PORT = 1000u;

	//used to filter messages to clients, for example if a message is meant only for players
	//on the same team. Add as many teams as required, names are arbitrary - just don't delete
	//or rename TEAM_ALL
	const sf::Uint8 TEAM_ALL = 0u;
	const sf::Uint8 TEAM_MIGHTY_DUCKS = 1u;
	const sf::Uint8 TEAM_TREDERIA = 2u;
	//we could use an enum but this saves an extra overload of the packet stream operators//

	//used to enable / disable IRC style commands
	const sf::Uint8 ENABLE_ME = 1u;
	const sf::Uint8 ENABLE_NICK = 1u;
	const sf::Uint8 ENABLE_TEAM = 1u;
	const sf::Uint8 ENABLE_CONNECT = 1u;
	const sf::Uint8 ENABLE_DISCONNECT = 1u;
	const sf::Uint8 ENABLE_KICK = 1u;
	const sf::Uint8 ENABLE_PLAY = 1u;

	//server message UIDs. These are broadcast as the UID of a packet from the server so the client knows
	//to parse a specific message
	const sf::Uint32 UID_KICK = 0xffffff00u; //server is requesting to disconnect the client
	//const sf::Uint32 UID_BAN = 0xffffff01u; //server sending a ban
	const sf::Uint32 UID_MOTD = 0xffffff02u; //server sending motd
	const sf::Uint32 UID_SOUND = 0xffffff03u; //server requesting client play sound
	const sf::Uint32 UID_ACTION = 0xffffff04u; //tells other clients when a client has performed an action
	const sf::Uint32 UID_TEAM = 0xffffff05u; //let clients know incoming message is a team message
	const sf::Uint32 UID_SERVER_MESSAGE = 0xffffff06u; //generic server message

	//default colours for output text
	static const sf::Color COLOUR_DEFAULT = sf::Color::White;
	static const sf::Color COLOUR_ACTION = sf::Color::Green;
	static const sf::Color COLOUR_TEAM = sf::Color::Blue;
	static const sf::Color COLOUR_SERVER_MESSAGE = sf::Color::Red;

	//////////////////////////////////////////////////////////////////////
	// Declarations
	//////////////////////////////////////////////////////////////////////

	//used to track the state of a chat object
	enum ChatState
	{
		NotConnected,
		ChatClient,
		ChatServer
	};

	//this is the object sent over the network
	//if adding any new members to this remember to update the overloads
	//for sf::packet << and >> operators also
	struct NetData
	{
		NetData(std::string name = "", std::string message = "", sf::Uint8 team = TEAM_ALL, sf::Uint32 uid = 0, std::string nickList = "")
			: name(name),
			message(message),
			team(team),
			uid(uid),
			nickList(nickList) {};

		std::string name, message, nickList;
		sf::Uint32 uid; //unique id of client
		sf::Uint8 team; //which team can see this message
	};

	//this is used to hold client data for a single connection so that
	//clients can be addressed individually when needed
	struct Client
	{
		Client()
			: socket(std::unique_ptr<sf::TcpSocket>(new sf::TcpSocket)), uid(0)
		{
			//generate a unique ID from location in memory
			uid = (sf::Uint32)socket.get();
		}
		std::unique_ptr<sf::TcpSocket> socket;
		sf::Uint32 uid;
		std::string name;
		std::string skipMessage; //we want to skip receiving the last message on the server
		//as it was received before we joined.
	};

	//by wrapping a string in a struct we can add other properties like colour
	struct TextLine
	{
		TextLine(std::string msg, sf::Color color = COLOUR_DEFAULT) :text(msg), colour(color){};
		std::string text;
		sf::Color colour;
	};


	//this is the main chat class which manages both client and server connections, and updating
	//text input and output buffers
	class Chat
	{
	public:
		//constructor
		Chat();
		//destructor
		~Chat();
		//must be called approx once a frame with the event object of the current window
		//to check for incoming keypress / text events
		void Update(sf::Event& event);

		//returns a string containing the output buffer. Set the number of lines with m_maxChatLines (sfchat.cpp)
		const std::string GetOutputText(void);
		//returns a string containing the current user input if input is enabled, or an empty string otherwise
		const std::string GetInputText(void);
		//returns a string listing all currently connected user names
		const std::string GetNickList(void);
		//returns a reference to the output buffer so text lines can be accessed individually
		const std::deque<TextLine>& GetOutputBuffer(void);

		//enables logging of chat to text file
		void EnableLogging(bool b = true);
		//enable time stamping of text when logging
		void EnableTimeStamp(bool b = true);
		//enable writing to the input
		void EnableInput();
		
		//get current state of chat - server, client or disconnected. See ChatState enumeration.
		const ChatState GetCurrentState(void);
		//returns true if input box is open (use this to disable game input while entering chat text)
		bool WaitingInput(void);

		//creates a server listening for connections on given port with text logging enabled or disabled
		void Create(sf::Uint16 port = DEFAULT_PORT, bool log = true);
		//creates a client and attempts to connect on given IP and port, with text logging enabled or disabled
		//connect attempt timeout is set via m_connectTimeout at the top of sfchat.cpp
		void Connect(sf::IpAddress address = sf::IpAddress("localhost"), sf::Uint16 port = DEFAULT_PORT, bool log = false);
		//stops a server running or disconnects a client from a server and returns the chat to a NotConnected
		//status, ready to create a new server or client
		void Disconnect(void);

		//sets the username of the current user. The server tracks user names by unique ID and will update
		//duplicate names if necessary. (Bear this in mind of the player's name is duplicated elsewhere in your game)
		void SetUserName(std::string name);
		//sets the team of a client, a client will only receieve messages addressed to this team, or all teams
		//does not affect server which receives all messages.
		void SetClientTeam(sf::Uint8 team);
		//sets the greeting message sent by server to connecting clients
		void SetMotd(std::string message);
		//attempts to execute an IRC style command string, does nothing when cmd is invalid
		void ExecIrcCmd(std::string cmd);

	private:
		//stores all the lines received ready for output
		std::deque<TextLine> m_outputBuffer;
		//concatenated string from the output buffer
		std::string m_outputString;
		//input string when user types a message
		std::string m_inputBuffer;
		//input buffer for clients to send to server
		std::string m_clientBuffer;
		//username of current user
		std::string m_username;
		//unique id of user, assigned by server
		sf::Uint32 m_uid;
		//current team the client belongs to, or team server is sending message to
		//(server sees all messages regardless for logging purposes)
		sf::Uint8 m_currentTeam;
		//greeting message to send to new connections
		std::string m_motd;
		//formatted string of currently connected users
		std::string m_nickList;
		//name of client to be kicked
		std::string m_kickRequest;
		//name of sound requested to be played
		std::string m_soundRequest;

		//unique authentication string - make this unique to the project sfchat is included in
		static const std::string m_authString;

		//whether we allow typing or not
		bool m_allowInput;
		//key to enable / disable text input
		static const sf::Keyboard::Key m_chatEnableKey;
		static const sf::Keyboard::Key m_chatDisableKey;
		//previous key pressed to prevent repeat keystrokes
		sf::Keyboard::Key m_lastKey;
		//max length of input buffer
		static const sf::Uint8 m_maxBufferLength;
		//max number of lines to show in chat window
		static const sf::Uint8 m_maxChatLines;
		//maximum length of username in chars
		static const sf::Uint8 m_maxNickLength;
		//prefix for input buffer
		static const std::string m_inputBufferPrefix;
		//current state of chat
		ChatState m_currentState;
		//if logging to text file is enabled
		bool m_loggingEnabled;
		//if timestamping chat enabled
		bool m_timestampEnabled;
		//port to run server on
		sf::Uint16 m_port;
		//ip addresss to attempt to connect to
		sf::IpAddress m_destIp;
		//flag marks if the client changed the name locally, and should ignore the server
		//if it tries to change it back again
		bool m_localNameChange;
		//if true message is sent only to clients on same team
		bool m_sendToTeam;
		//used for flashing cursor effect
		sf::Clock m_cursorClock;


		//***members for threading***//
		std::unique_ptr<sf::Thread> m_netThread; //runs the network function for client/server
		std::unique_ptr<sf::Thread> m_consoleThread; //runs a thread to handle console input
		sf::Mutex m_mutex;
		bool m_runThread;
		//safely kills a thread by waiting for it to quit. Use this to stop
		//a running client or server and return to a non-connected status.
		//DO NOT attempt to call this from a client or server thread.
		void m_KillThread(void);

		//time members for time stamping messages
		time_t m_timeNow;
		tm* m_localTime;

		//members for logging chat to a text file
		std::ofstream m_logFile;

		//function to run in server thread
		void m_Server(void);
		//function to run in client thread
		void m_Client(void);
		//maximum number of clients allowed to connect
		static const sf::Uint8 m_maxClients;
		//number of seconds to try connecting to a server before timing out
		static const float m_connectTimeout;
		//function to run in console thread
		void m_ConsoleInput(void);

		//processes the input buffer and sends the result to the output
		void m_ProcessInput(void);
		//adds the specified string to the local output buffer
		void m_OutputString(std::string str, sf::Color colour = COLOUR_DEFAULT);
		//updates the output string with current contents of m_outputBuffer
		void m_UpdateOutput(void);
		//parses any IRC style strings and returns the result. Result is empty
		//if command is no known.
		std::string m_ParseCommand(std::string str);
		//function to update duplicate names with random number
		void m_RandName(std::string& name);
		//updates the nick list property on the server
		void m_UpdateNickList(std::list< std::shared_ptr<Client> >& clients);
		//plays an audio file on disk. Define this yourself to use your audio engine
		void m_PlaySound(std::string sound);
	};


};

#endif