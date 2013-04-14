#include "../../include/game/Settings.hpp"
#include "../../include/aw/utilities/Converter.hpp"

#include <fstream>

namespace settings
{
    namespace
    {
        // Game settings
        bool m_music = true;
        int  m_musicVolume = 100;
        unsigned int m_antialiasing = 0;
        int m_timeForTracer = 800;
        // Level progression
        std::vector<std::string> m_levelList;
        int m_unlocked;
        // Settings for the current level
        GameMode m_gamemode = ARCADE;

        float m_speedX;
        float m_speedY;

        float m_startSpeedX;
        float m_downforce;
        float m_startDownforce;

        int m_musicName = -1;
    }

    void Load()
    {
        std::fstream file;
        file.open("config/options.cfg", std::ios::in);

        std::string line;

        while(!file.eof())
        {
            std::getline(file, line);

            if(line == "[Music]")
            {
                file.eof();
                std::getline(file, line);

                if(line == "off")
                    m_music = false;
                else
                    m_music = true;
            }

            if(line == "[Volume]")
            {
                file.eof();
                std::getline(file, line);

                m_musicVolume = aw::conv::ToInt(line);

                if(m_musicVolume > 100 || m_musicVolume < 0)
                    m_musicVolume = 50;
            }

            if(line == "[Antialiasing]")
            {
                file.eof();
                std::getline(file, line);

                m_antialiasing = aw::conv::ToInt(line);
            }
            if(line == "[TimeForTracer]")
            {
                file.eof();
                std::getline(file, line);

                m_timeForTracer = aw::conv::ToInt(line);
            }

            if(line == "[Levellist]")
            {
                while(!file.eof())
                {
                    std::getline(file, line);

                    if(line == "[/Levellist]")
                        break;

                    m_levelList.push_back(line);
                }
            }

            if(line == "[Unlocked]")
            {
                file.eof();
                std::getline(file, line);

                int encrypted = aw::conv::ToInt(line);
                m_unlocked = (encrypted + 140) / 2000;
            }

        }

        file.close();

    }

    void Save()
    {
        std::fstream file;
        file.open("config/options.cfg", std::ios::out | std::ios::trunc);

        file << "Options for Kroniax\n";
        file << "[Music]\n";

        if(m_music)
            file << "on\n";
        else
            file << "off\n";

        file << "[Volume]\n";
        file << m_musicVolume << "\n";

        file << "[Antialiasing]\n";
        file << m_antialiasing << "\n";

        file << "[TimeForTracer]\n";
        file << m_timeForTracer << "\n";

        file << "[Levellist]\n";

        for(unsigned int i = 0; i < m_levelList.size(); i++)
        {
            file << m_levelList[i] << "\n";
        }

        file << "[/Levellist]\n\n";

        file << "[Unlocked]\n";
        file << (m_unlocked*2000)-140 << "\n";

        file.close();

    }

    bool IsMusicOn()
    {
        return m_music;
    }

    int GetMusicVolume()
    {
        return m_musicVolume;
    }

    void EnableMusic(bool value)
    {
        m_music = value;
    }

    void AdjustVolume(int value)
    {
        m_musicVolume = value;
    }
    void SetAntialiasing(unsigned int value)
    {
        m_antialiasing = value;
    }
    unsigned int GetAntialiasing()
    {
        return m_antialiasing;
    }
    void SetTimeForTracer(int milliseconds)
    {
        m_timeForTracer = milliseconds;
    }
    int GetTimeForTracer()
    {
        return m_timeForTracer;
    }

    void SetGamemode(const GameMode& value)
    {
        m_gamemode = value;
    }

    GameMode GetGamemode()
    {
        return m_gamemode;
    }

    std::vector<std::string> &GetLevelList()
    {
        return m_levelList;
    }

    int GetUnlockedLevel()
    {
        return m_unlocked;
    }

    bool IncreaseUnlockLevel()
    {
        if(m_unlocked < static_cast<int>(m_levelList.size()))
        {
            m_unlocked++;

            return true;
        }

        return false;
    }

    float GetSpeedX()
    {
        return m_speedX;
    }
    float GetSpeedY()
    {
        return m_speedY;
    }
    void SetSpeedX(float value)
    {
        m_speedX = value;
    }
    void SetSpeedY(float value)
    {
        m_speedY = value;
    }
    void SetStartSpeedX(float value)
    {
        m_startSpeedX = value;
    }
    void SetDownforce(float value)
    {
        m_downforce = value;
    }
    void SetStartDownforce(float value)
    {
        m_startDownforce = value;
    }
    float GetStartSpeedX()
    {
        return m_startSpeedX;
    }
    float GetDownforce()
    {
        return m_downforce;
    }
    float GetStartDownforce()
    {
        return m_startDownforce;
    }
    void SetMusic(int index)
    {
        m_musicName = index;
    }
    int GetMusic()
    {
        return m_musicName;
    }

}
