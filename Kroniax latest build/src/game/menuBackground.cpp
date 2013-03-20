#include "../../include/game/menuBackground.hpp"

MenuBackground::MenuBackground(sf::RenderWindow &window) : m_window(window)
{
    m_renderTex.create(window.getView().getSize().x, window.getView().getSize().y, true);
    m_toDraw.setTexture(m_renderTex.getTexture());


    m_view = m_window.getView();

    m_view.setCenter(400,225);

    m_renderTex.setView(m_view);
}

void MenuBackground::Init()
{
    int lvlnumber = (rand() % 8) + 1;

    m_path = ("levels/Level"+aw::conv::ToString(lvlnumber)+"/");

    m_textures.clear();
    m_tiles.clear();

    if(!LoadTextures())
    {
        return;
    }

    LoadTileData(1);

    m_tilesPerRow = 800 / m_tileSize.x;

}

void MenuBackground::DoLogic()
{
    m_view.move(4,0);

    if(m_view.getCenter().x >= m_tileSize.x * m_levelLength)
    {
         m_view = m_window.getView();

        m_view.setCenter(400,225);

        m_renderTex.setView(m_view);
        Init();
    }

    m_renderTex.setView(m_view);
}

void MenuBackground::Draw()
{

    m_renderTex.clear();

    int startTile = static_cast<int>((m_view.getCenter().x - 400) / m_tileSize.x) * m_tilesPerCollum;
    unsigned int endTile = startTile + ( (m_tilesPerRow+1) * m_tilesPerCollum);

    if(endTile > m_tiles.size())
        endTile = m_tiles.size();


    for(unsigned int i = startTile; i < endTile; i++)
    {
        m_renderTex.draw(m_tiles[i]);
    }


    m_window.draw(m_toDraw);
}

bool MenuBackground::LoadTextures()
{
    std::stringstream sstr;
    sstr << m_path << "tilesets.cfg";

    std::fstream file;
    file.open(sstr.str().c_str(), std::ios::in);

    std::string line;
    int numberOfTex = 0;

    if(file.fail())
    {
        return false;
    }

    while(!file.eof())
    {
        std::getline(file, line);

        if(line == "[Size]")
        {
            file.eof();
            std::getline(file, line);
            std::string substr;
            for(unsigned int i = 0; i < line.size(); i++)
            {
                if(line[i] != '|')
                {
                    substr = substr + line[i];
                }else
                {
                    m_tileSize.x = aw::conv::ToInt(substr);
                    substr.clear();
                }
            }
            m_tileSize.y = aw::conv::ToInt(substr);
        }



        if(line == "[Lenght]")
        {
            file.eof();
            std::getline(file, line);
            m_levelLength = aw::conv::ToInt(line);
        }

        if(line == "[Height]")
        {
            file.eof();
            std::getline(file, line);
            m_levelHeight = aw::conv::ToInt(line);
        }

        if(line == "[Number of Tileset]")
        {
            file.eof();
            std::getline(file, line);
            std::stringstream sstri;
            sstri << line;
            sstri >> numberOfTex;
        }

        if(line == "[Paths]")
        {
            for(int i = 0; i < numberOfTex; i++)
            {
                file.eof();
                std::getline(file, line);

                sf::Texture tex;
                tex.loadFromFile(line);
                tex.setSmooth(true);
                m_textures.push_back(tex);
            }
        }

    }

    return true;
}

void MenuBackground::LoadTileData(int layer)
{
    std::stringstream sstr;
    sstr << m_path << "tileData" << layer << ".cfg";

    std::fstream file;
    file.open(sstr.str().c_str(), std::ios::in);

    std::string line;

    while(!file.eof())
    {
        std::getline(file, line);

        if(line == "[Data]")
        {
            for(int i = 0; i < m_levelLength; i++)
            {

                file.eof();
                std::getline(file, line);

                int temp = -1;

                for(unsigned int j = 0; j < line.size(); j++)
                {
                    std::string tempNumTex = "";
                    std::string tempXCoord = "";
                    std::string tempYCoord = "";
                    std::string tempRotation = "";

                    if(line[j] == '(')
                    {
                        temp++;
                        for(unsigned int h = j+1; h < line.size(); h++)
                        {
                            if(line[h] != '|')
                            {
                                tempNumTex = tempNumTex + line[h];
                            }else
                            {
                                for(unsigned int g = h+1; g < line.size(); g++)
                                {
                                    if(line[g] != '|')
                                    {
                                        tempXCoord = tempXCoord + line[g];
                                    }else{
                                        for(unsigned int l = g+1; l < line.size(); l++)
                                        {
                                            if(line[l] != '|')
                                            {
                                                tempYCoord = tempYCoord + line[l];
                                            }else
                                            {
                                                for(unsigned int r = l+1; r < line.size(); r++)
                                                {
                                                    if(line[r] != ')')
                                                    {
                                                        tempRotation = tempRotation + line[r];
                                                    }else
                                                    {
                                                        break;
                                                    }
                                                }
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                        if(aw::conv::ToInt(tempNumTex) == -1)
                        {
                            m_tiles.push_back(sf::Sprite());
                        }else
                        {
                            sf::Sprite sprite;
                            sprite.setTexture(m_textures[aw::conv::ToInt(tempNumTex)]);

                            sprite.setTextureRect(sf::IntRect(aw::conv::ToInt(tempXCoord),
                                                              aw::conv::ToInt(tempYCoord),
                                                              m_tileSize.x,
                                                              m_tileSize.y));

                            m_tiles.push_back(sprite);

                        }
                    }
                }
            }
        }
    }

    m_tilesPerCollum = (m_levelHeight / m_tileSize.y);

    for(unsigned int i = 0; i < m_tiles.size(); i++)
    {

        m_tiles[i].setPosition(static_cast<int>(i / (m_levelHeight / m_tileSize.y)) * m_tileSize.x,
                                       m_tileSize.y * (i - (static_cast<int>(i/m_tilesPerCollum)*m_tilesPerCollum)) );


    }

}

