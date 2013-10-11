#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "../messageBus/receiver.hpp"

/////FORWARD DECLARATION/////
namespace aw
{
	class MessageBus;
}

namespace aw
{
	//Settings structs
	namespace settings
	{
		struct WindowSettings
		{
			std::string name;
			unsigned int windowSizeX, windowSizeY;
			bool fullscreen;
			unsigned int antialiasingLevel, majorVersion, minorVersion;
			float viewWidth, viewHeight;
		};
		struct SoundSettings
		{
			float masterVolume;
		};
		struct ArcadeSettings
		{
			unsigned int unlockValue;
			std::vector<std::string> levelList;
		};
	}

	class Settings : public Receiver
	{
	public:
		Settings(MessageBus &messageBus);

		void load();
		void save();

		void receiveMessage(const Message &msg);

	private:

		void sendArcadeSettings();

		void loadWindowAndSoundSettings();
		void loadArcadeSettings();

		void unlockNewLevel(const std::string &name);
		void sendNextLevel(const std::string &currentLevel);

	private:
		aw::MessageBus &m_messageBus;

		settings::WindowSettings m_windowSettings;
		settings::SoundSettings m_soundSettings;
		settings::ArcadeSettings m_arcadeSettings;
	};
}

#endif