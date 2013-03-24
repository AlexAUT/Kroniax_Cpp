#include "../../include/game/HighscoreUploader.hpp"

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Socket.hpp>

HighscoreUploader::HighscoreUploader()
{

}

bool HighscoreUploader::Submit(std::string lvlname, std::string name, unsigned int length, unsigned int filledBlocks, unsigned int collisionBlocks, unsigned int score)
{

    sf::TcpSocket server;

    sf::Packet packet;

    short unsigned port = 60600;
    sf::IpAddress serverAddress("82.211.56.205");
    //sf::IpAddress serverAddress("127.0.0.1");
    packet << "highscore";
    packet << lvlname << name << length << filledBlocks << collisionBlocks << score;

    //Counts the connection attempts
    int numberOfRetries = 0;

    while(server.connect(serverAddress, port) != sf::Socket::Done)
    {
        numberOfRetries++;

        if(numberOfRetries > 2)
        {
            //return false if the connection failed after 3 retries
            return false;
        }
    }

    server.send(packet);

    sf::Packet rec;

    server.receive(rec);

    std::string command;
    rec >> command;


    if(command == "highscore")
    {
        unsigned int numberOfEntries;

        rec >> numberOfEntries;



        for(unsigned int i = 0; i < numberOfEntries; i++)
        {
            Score temp;
            rec >> temp.score >> temp.name;
            m_score.push_back(temp);
        }
        return true;
    }

    else if(command == "error")
    {
        rec >> command;
    }

    server.disconnect();

    return false;

}

bool HighscoreUploader::GetHighscore(std::string levelname)
{
    sf::TcpSocket server;

    sf::Packet packet;

    short unsigned port = 60600;
    sf::IpAddress serverAddress("82.211.56.205");
    //sf::IpAddress serverAddress("127.0.0.1");
    packet << "getHighscore";
    packet << levelname;


    //Counts the connection attempts
    int numberOfRetries = 0;

    while(server.connect(serverAddress, port) != sf::Socket::Done)
    {
        numberOfRetries++;

        if(numberOfRetries > 2)
        {
            //return false if the connection failed after 3 retries
            return false;
        }
    }

    server.send(packet);

    sf::Packet rec;

    server.receive(rec);

    std::string command;
    rec >> command;


    if(command == "highscore")
    {
        unsigned int numberOfEntries;

        rec >> numberOfEntries;

        for(unsigned int i = 0; i < numberOfEntries; i++)
        {
            Score temp;
            rec >> temp.score >> temp.name;
            m_score.push_back(temp);
        }
        return true;
    }

    return false;

}


std::vector<Score>& HighscoreUploader::GetScore()
{
    return m_score;
}
