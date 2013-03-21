#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <memory>

namespace sf
{
	class RenderWindow;
}

#include <vector>
#include <string>

struct Script
{
    Script() : m_used(false)
    {

    }

    bool m_used;
    int m_type;
    int m_value;
    int m_posX;
};


class Level
{
public:
    Level(sf::RenderWindow &window);

    void SetWindow(sf::RenderWindow &window);

    bool Load(std::string name);

    bool Collide(int viewCenterX, sf::Vector2i p1, sf::Vector2i p2, sf::Vector2i p3);

    void CheckForScripts(int posX);

    bool GetFinished();

    void Draw(int viewCenterX);


    int GetLevelLength();
    sf::Vector2f GetTileSize();


    unsigned int GetFilledBlocks();
    unsigned int GetCollisionBlocks();

private:

    bool LoadTextures();
    void LoadTileData(int layer);

    void LoadCollideGrid();

    void LoadScripts();

private:

    sf::RenderWindow &m_window;

    std::string m_path;

    std::vector<std::unique_ptr<sf::Texture>> m_textures;
    std::vector<sf::Sprite> m_tiles;
    std::vector<int> m_collision;

    std::vector<Script> m_scripts;

    int m_levelLength;
    int m_levelHeight;
    int m_tilesPerCollum;
    int m_tilesPerRow;
    sf::Vector2f m_tileSize;

    float scrollX;

    bool m_finished;

    unsigned int m_filledBlocks;
    unsigned int m_collisionBlocks;
};

#endif
