#include "../../include/game/CustomMapManager.hpp"

#include "../../include/game/DialogScreen.hpp"
#include "../../include/aw/utilities/Converter.hpp"


#include <iostream>
#include <fstream>

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/System/Time.hpp>


#ifdef _WIN32
    #include <direct.h>
#endif // _WIN32

#ifdef __linux__
 #include <sys/stat.h>
 #include <sys/types.h>
#endif // __linux__


CustomMapManager::CustomMapManager()
{

}


void CustomMapManager::LoadMapList()
{
    m_mapList.clear();

    std::string path = "levels/levellist.cfg";

    std::fstream file;
    file.open(path.c_str(), std::ios::in);

    if(file.fail())
        return;

    std::string line;

    while(!file.eof())
    {
        std::getline(file, line);

        if(line == " " || line == "")
            continue;

        m_mapList.push_back(Map());
        m_mapList[m_mapList.size()-1].name = line;

        LoadSettings(line);
    }

    file.close();
}


bool CustomMapManager::UpdateMapList()
{
    int port = 60602;
    //sf::IpAddress serverAddress("127.0.0.1");
    sf::IpAddress serverAddress("82.211.56.205");

    sf::TcpSocket server;

    int countRetries = 0;

    while(server.connect(serverAddress, port, sf::seconds(1)) != sf::Socket::Done)
    {
        countRetries++;
           std::cout << "hier" << std::endl;
        if(countRetries > 3)
        {
            return false;
        }
    }

    sf::Packet packet;
    packet << "levellist";
    server.send(packet);
    packet.clear();

    server.receive(packet);

    std::string path = "levels/levellist.cfg";
    std::fstream file(path.c_str(), std::ios::out |std::ios::trunc);

    while(!packet.endOfPacket())
    {
        std::string line;
        packet >> line;
        if(line != "" && line != " ")
            file << line << "\n";
    }

    file.close();
    server.disconnect();

    return true;
}

bool CustomMapManager::DownloadMapDescription(std::string name)
{
    int port = 60602;
    //sf::IpAddress serverAddress("127.0.0.1");
    sf::IpAddress serverAddress("82.211.56.205");
    sf::TcpSocket server;

    int countRetries = 0;

    while(server.connect(serverAddress, port, sf::seconds(1)) != sf::Socket::Done)
    {
        countRetries++;

        if(countRetries > 3)
        {
            return false;
        }
    }

    sf::Packet packet;
    packet << "getdesc" << name;
    server.send(packet);

    packet.clear();
    server.receive(packet);

    std::string path = "levels/"+name;


    #ifdef _WIN32
        mkdir(path.c_str());
    #endif // _WIN32
    #ifdef __linux__
        mkdir(path.c_str(), 0777);
    #endif // __linux__

    std::string filename;
    packet >> filename;

    if(filename == "failed")
    {
        server.disconnect();
        return false;
    }

    path = path + "/"+filename;
    std::fstream file(path.c_str(), std::ios::out | std::ios::trunc);

    while(!packet.endOfPacket())
    {
        std::string line;
        packet >> line;
        file << line << "\n";
    }

    file.close();
    server.disconnect();

    LoadSettings(name);

    return true;
}

bool CustomMapManager::DownloadMap(std::string name)
{
    int port = 60602;
    //sf::IpAddress serverAddress("127.0.0.1");
    sf::IpAddress serverAddress("82.211.56.205");
    sf::TcpSocket server;

    int countRetries = 0;

    while(server.connect(serverAddress, port, sf::seconds(1)) != sf::Socket::Done)
    {
        countRetries++;

        if(countRetries > 3)
        {
            return false;
        }
    }
    sf::Packet packet;
    packet << "getmap" << name;
    server.send(packet);
    packet.clear();


    for(int i = 0; i < 3; i++)
    {
        server.receive(packet);
        std::string filename;
        packet >> filename;

        std::string path = "levels/"+name+"/"+filename;

        std::fstream file(path.c_str(), std::ios::out | std::ios::trunc);

        while(!packet.endOfPacket())
        {
            std::string line;
            packet >> line;
            file << line << "\n";
        }
        file.close();
    }


    LoadSettings(name);

    server.disconnect();

    return true;

}




Map *CustomMapManager::GetMap(unsigned int index)
{
    if(index < m_mapList.size())
        return &m_mapList[index];

    return nullptr;
}

Map *CustomMapManager::GetMap(std::string name)
{
    for(unsigned int i = 0; i < m_mapList.size(); i++)
    {
        if(m_mapList[i].name == name)
            return &m_mapList[i];
    }

    return nullptr;
}


unsigned int CustomMapManager::GetMapCount()
{
    return m_mapList.size();
}




bool CustomMapManager::LoadSettings(std::string name)
{
    std::string path = "levels/"+name+"/settings.cfg";

    std::fstream file(path.c_str(), std::ios::in);

    if(file.fail())
    {
        return false;
    }

    std::string line;
    while(!file.eof())
    {
        std::getline(file, line);

        if(line == "[Author]")
        {
            file.eof();
            std::getline(file, line);
            GetMap(name)->author = line;
        }
        if(line == "[Gamemode]")
        {
            file.eof();
            std::getline(file, line);
            GetMap(name)->gameMode = line;
        }
        if(line == "[Difficulty]")
        {
            file.eof();
            std::getline(file, line);
            GetMap(name)->difficulty = line;
        }
        if(line == "[SpeedX]")
        {
            file.eof();
            std::getline(file, line);
            GetMap(name)->speedX = aw::conv::ToInt(line);
        }
        if(line == "[Length]")
        {
            file.eof();
            std::getline(file, line);
            GetMap(name)->length = aw::conv::ToInt(line);
        }

    }

    return true;
}
