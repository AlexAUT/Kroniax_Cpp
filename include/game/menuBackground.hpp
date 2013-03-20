#ifndef MENUm_BACKGROUND
#define MENUm_BACKGROUND

#include "settings.hpp"

#include "../../include/aw/utilities/converter.hpp"

#include <fstream>

class MenuBackground
{
public:

    MenuBackground(sf::RenderWindow &window);

    void Init();

    void DoLogic();

    void Draw();

private:

    bool LoadTextures();
    void LoadTileData(int layer);

private:

    sf::View m_view;

    sf::RenderWindow &m_window;

    sf::RenderTexture m_renderTex;
    sf::Sprite        m_toDraw;

    std::vector<sf::Texture> m_textures;
    std::vector<sf::Sprite> m_tiles;

    std::string m_path;

    int m_levelLength;
    int m_levelHeight;
    int m_tilesPerCollum;
    int m_tilesPerRow;
    sf::Vector2f m_tileSize;

    float scrollX;

};

#endif // MENUm_BACKGROUND
