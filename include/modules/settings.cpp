#include "settings.hpp"

#include <fstream>
#include <string>
#include <sstream>

/////INCLUDE FORWARD DECLARATIONS/////
#include "../messageBus/messageBus.hpp"

namespace aw
{
	Settings::Settings(MessageBus &messageBus) :
		m_messageBus(messageBus)
	{
		m_messageBus.addReceiver(this);
	}

	void Settings::load()
	{
		loadWindowAndSoundSettings();
		loadArcadeSettings();
	}

	void Settings::save()
	{
		std::fstream file("data/config.cfg", std::ios::out | std::ios::trunc);
		file << "Config file for the game Kroniax\n";

		//Save window settings
		file << "[Window]\n";
		file << m_windowSettings.name << " ";
		file << m_windowSettings.windowSizeX << " ";
		file << m_windowSettings.windowSizeY << " ";
		file << m_windowSettings.fullscreen << " ";
		file << m_windowSettings.antialiasingLevel << " ";
		file << m_windowSettings.majorVersion << " ";
		file << m_windowSettings.minorVersion << std::endl;

		//Save view settings
		file << "[View]\n";
		file << m_windowSettings.viewWidth << " ";
		file << m_windowSettings.viewHeight << std::endl;

		//Save sound settings
		file << "[Sound]\n";
		file << m_soundSettings.masterVolume << std::endl;
	}

	void Settings::loadWindowAndSoundSettings()
	{
		std::fstream file("data/config.cfg", std::ios::in);
		std::string line;

		while (std::getline(file, line))
		{
			if (line == "[Window]")
			{
				//Window settings
				std::getline(file, line);
				std::stringstream sstr(line);
				sstr >> m_windowSettings.name;
				sstr >> m_windowSettings.windowSizeX >> m_windowSettings.windowSizeY;
				sstr >> m_windowSettings.fullscreen;
				sstr >> m_windowSettings.antialiasingLevel >> m_windowSettings.majorVersion >> m_windowSettings.minorVersion;
			}
			else if (line == "[View]")
			{
				//View dim
				std::getline(file, line);
				std::stringstream sstr(line);
				sstr >> m_windowSettings.viewWidth >> m_windowSettings.viewHeight;
			}
			else if (line == "[Sound]")
			{
				//Sound settings
				std::getline(file, line);
				std::stringstream sstr(line);
				sstr >> m_soundSettings.masterVolume;
			}
		}

		file.close();

		//Sending window properties to the MessageBus
		Message msgWindow;
		msgWindow.ID = std::hash<std::string>()("window settings");
		msgWindow.push_back(m_windowSettings.name);
		msgWindow.push_back(m_windowSettings.windowSizeX);
		msgWindow.push_back(m_windowSettings.windowSizeY);
		msgWindow.push_back(m_windowSettings.fullscreen);
		msgWindow.push_back(m_windowSettings.antialiasingLevel);
		msgWindow.push_back(m_windowSettings.majorVersion);
		msgWindow.push_back(m_windowSettings.minorVersion);
		msgWindow.push_back(m_windowSettings.viewWidth);
		msgWindow.push_back(m_windowSettings.viewHeight);
		m_messageBus.sendMessage(msgWindow);

		//Sending sound settings to the MessageBus
		Message msgSound;
		msgSound.ID = std::hash<std::string>()("sound settings");
		msgSound.push_back(m_soundSettings.masterVolume);
		m_messageBus.sendMessage(msgSound);

	}

	void Settings::loadArcadeSettings()
	{
		std::fstream file("data/arcade levellist.cfg", std::ios::in);
		std::string line;

		//First line = unlocked levels
		std::getline(file, line);
		std::stringstream sstr(line);
		sstr >> m_arcadeSettings.unlockValue;
		sstr.str("");

		//Now read the levelList
		while (std::getline(file, line))
		{
			m_arcadeSettings.levelList.push_back(line);
		}
		file.close();
		//Send the unlock information
		Message msgUnlock;
		msgUnlock.ID = std::hash<std::string>()("unlocked levels");
		msgUnlock.push_back<unsigned int>(m_arcadeSettings.unlockValue);
		m_messageBus.sendMessage(msgUnlock);

		//Send level list
		Message msgLvllist;
		msgLvllist.ID = std::hash<std::string>()("arcade levellist");
		for (auto &it : m_arcadeSettings.levelList)
		{
			msgLvllist.push_back<std::string>(it);
		}
		m_messageBus.sendMessage(msgLvllist);
	}

	void Settings::receiveMessage(const Message &msg)
	{
		//Window resize
		if (msg.ID == std::hash<std::string>()("window resized"))
		{
			m_windowSettings.windowSizeX = *msg.getValue<unsigned int>(0);
			m_windowSettings.windowSizeY = *msg.getValue<unsigned int>(1);
		}
	}
}