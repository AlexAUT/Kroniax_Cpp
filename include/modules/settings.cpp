#include "settings.hpp"

#include <fstream>
#include <sstream>

#include "../utilities/hash.hpp"
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

		//Save menu volume
		file << "[Menu Volume]\n";
		file << m_soundSettings.menuvolume << std::endl;

		//Save game volume
		file << "[Game Volume]\n";
		file << m_soundSettings.gamevolume << std::endl;

		file.close();

		//Arcade levellist
		file.open("data/arcade levellist.cfg", std::ios::out | std::ios::trunc);

		file << m_arcadeSettings.unlockValue << "\n";
		for (auto &it : m_arcadeSettings.levelList)
		{
			file << it << "\n";
		}

		file.close();
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
			else if (line == "[Menu Volume]")
			{
				//Menu Volume
				std::getline(file, line);
				std::stringstream sstr(line);
				sstr >> m_soundSettings.menuvolume;
			}
			else if (line == "[Game Volume]")
			{
				//Game volume
				std::getline(file, line);
				std::stringstream sstr(line);
				sstr >> m_soundSettings.gamevolume;
			}
		}

		file.close();

		sendWindowSettings();
		sendSoundSettings();
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

		//Send information
		sendArcadeSettings();
	}

	void Settings::sendSoundSettings()
	{
		//Sending sound settings to the MessageBus
		Message msgSound;
		msgSound.ID = aw::hash("sound settings");
		msgSound.push_back(m_soundSettings.menuvolume);
		msgSound.push_back(m_soundSettings.gamevolume);
		m_messageBus.sendMessage(msgSound);
	}

	void Settings::sendWindowSettings()
	{
		//Sending window properties to the MessageBus
		Message msgWindow;
		msgWindow.ID = aw::hash("window settings");
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
	}

	void Settings::sendArcadeSettings()
	{
		//Send the unlock information
		Message msgUnlock;
		msgUnlock.ID = aw::hash("unlocked levels");
		msgUnlock.push_back<unsigned int>(m_arcadeSettings.unlockValue);
		m_messageBus.sendMessage(msgUnlock);

		//Send level list
		Message msgLvllist;
		msgLvllist.ID = aw::hash("arcade levellist");
		for (auto &it : m_arcadeSettings.levelList)
		{
			msgLvllist.push_back<std::string>(it);
		}
		m_messageBus.sendMessage(msgLvllist);
	}

	void Settings::unlockNewLevel(const std::string &name)
	{
		for (std::size_t i = 0; i < m_arcadeSettings.levelList.size(); i++)
		{
			if (name == m_arcadeSettings.levelList[i])
			{
				if (m_arcadeSettings.unlockValue <= i)
				{
					m_arcadeSettings.unlockValue = i + 1;
				}
				break;
			}
		}

		sendArcadeSettings();
	}

	void Settings::sendNextLevel(const std::string &currentLevel)
	{
		Message msg;
		msg.ID = aw::hash("start game");
		

		//Insert the next map
		for (auto it = m_arcadeSettings.levelList.begin(); it != m_arcadeSettings.levelList.end(); ++it)
		{
			if (*it == currentLevel)
			{
				msg.push_back(*++it);
			}
		}
		//Add gamemode
		msg.push_back(static_cast<std::string>("official arcade"));
		//Send message
		m_messageBus.sendMessage(msg);
	}

	void Settings::receiveMessage(const Message &msg)
	{
		//Window resize
		if (msg.ID == aw::hash("window resized"))
		{
			m_windowSettings.windowSizeX = *msg.getValue<unsigned int>(0);
			m_windowSettings.windowSizeY = *msg.getValue<unsigned int>(1);
		}
		else if (msg.ID == aw::hash("level complete"))
		{
			//Update unlocked levels
			unlockNewLevel(*msg.getValue<std::string>(0));
			//Save the progress
			save();

			//Check if the game modules wants to start the next level
			if (*msg.getValue<bool>(1))
			{
				//Send command to load the next level
				sendNextLevel(*msg.getValue<std::string>(0));
			}
		}
		else if (msg.ID == aw::hash("new sound settings"))
		{
			std::stringstream sstr(*msg.getValue<std::string>(0));
			sstr >> m_soundSettings.menuvolume;
			sstr.str("");
			
			std::stringstream sstr2;
			sstr2 << *msg.getValue<std::string>(1);
			sstr2 >> m_soundSettings.gamevolume;

			sendSoundSettings();
		}
		else if (msg.ID == aw::hash("new window settings"))
		{
			//Antialiasinglevel
			std::stringstream sstr(*msg.getValue<std::string>(0));
			sstr >> m_windowSettings.antialiasingLevel;

			//Fullscreen
			if (*msg.getValue<std::string>(1) == "off")
			{
				m_windowSettings.fullscreen = false;
			}
			else
			{
				m_windowSettings.fullscreen = true;
			}

			sendWindowSettings();
		}
		else if (msg.ID == aw::hash("reset resolution"))
		{
			m_windowSettings.windowSizeX = 800;
			m_windowSettings.windowSizeY = 450;
			sendWindowSettings();
		}
	}
}