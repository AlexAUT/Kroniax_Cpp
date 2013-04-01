#ifndef CUSTOMMAPMANAGER_HPP
#define CUSTOMMAPMANAGER_HPP

#include <string>
#include <vector>

//
// This class manage the curstommaps.
// It will download the levellist from the masterserver
// Will store informations about all downloaded and
// available maps.

struct Map
{
    Map() : name("unnamed"),
            author("not downloaded"),
            gameMode("not downloaded"),
            difficulty("not downloaded"),
            speedX(0),
            length(0)
    {

    }

    std::string name;
    std::string author;
    std::string gameMode;
    std::string difficulty;
    int speedX;
    int length;
};

class CustomMapManager
{
public:

    CustomMapManager();

    void LoadMapList();
    bool UpdateMapList();

    bool DownloadMapDescription(std::string name);
    bool DownloadMap(std::string name);

    Map *GetMap(unsigned int index);
    Map *GetMap(std::string name);
    unsigned int GetMapCount();

private:

    bool LoadSettings(std::string name);

private:

    std::vector<Map> m_mapList;



};

#endif // CUSTOMMAPMANAGER_HPP
