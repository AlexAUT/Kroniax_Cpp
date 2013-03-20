#include "../../include/game/level.hpp"

#define WINDOWHALFWIDTH 300


Level::Level(sf::RenderWindow &window) : m_window(window), m_finished(false), m_filledBlocks(15680451), m_collisionBlocks(14125125)
{

}


bool Level::Load(std::string name)
{
    m_path = "levels/"+name+"/";


    if(!LoadTextures())
    {

        return false;
    }

    LoadTileData(1);

    LoadCollideGrid();

    LoadScripts();

    m_tilesPerRow = 800 / m_tileSize.x;


    return true;

}

bool CollidePointRect(sf::Vector2i point, sf::Vector2f pos, sf::Vector2f size)
{
    if(point.x >= pos.x)
    {
        if(point.x <= pos.x + size.x)
        {
            if(point.y >= pos.y)
            {
                if(point.y <= pos.y + size.y)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Level::Collide(int viewCenterX, sf::Vector2i p1, sf::Vector2i p2, sf::Vector2i p3)
{
    int startTile = static_cast<int>((viewCenterX - WINDOWHALFWIDTH) / m_tileSize.x) * m_tilesPerCollum;
    unsigned int endTile = startTile + ( (m_tilesPerRow+1) * m_tilesPerCollum);

    for(unsigned int i = startTile;  i < endTile; i++)
    {
        if(m_collision[i] == 1)
        {
            int posX = static_cast<int>(i/m_tilesPerCollum) * m_tileSize.x;
            int posY = m_tileSize.y * (i - (static_cast<int>(i/m_tilesPerCollum)*m_tilesPerCollum));

            if(CollidePointRect(p1, sf::Vector2f(posX, posY), sf::Vector2f(25,25)))
                return true;

            if(CollidePointRect(p2, sf::Vector2f(posX, posY), sf::Vector2f(25,25)))
                return true;

            if(CollidePointRect(p3, sf::Vector2f(posX, posY), sf::Vector2f(25,25)))
                return true;
        }
        if(m_collision[i] == 2)
        {
            int posX = static_cast<int>(i/m_tilesPerCollum) * m_tileSize.x;
            int posY = m_tileSize.y * (i - (static_cast<int>(i/m_tilesPerCollum)*m_tilesPerCollum));

            if(CollidePointRect(p1, sf::Vector2f(posX, posY), sf::Vector2f(25,25)))
                m_finished = true;

            if(CollidePointRect(p2, sf::Vector2f(posX, posY), sf::Vector2f(25,25)))
                m_finished = true;

            if(CollidePointRect(p3, sf::Vector2f(posX, posY), sf::Vector2f(25,25)))
                m_finished = true;
        }
    }

    return false;
}

void Level::CheckForScripts(int xPos)
{
    if(m_scripts.size() < 1)
        return;

    for(auto p = m_scripts.begin(); p != m_scripts.end(); ++p)
    {
        if(!p->m_used)
        {
            if(xPos >= p->m_posX)
            {
                p->m_used = true;

                switch(p->m_type)
                {
                case 1:
                    if(settings::GetGamemode() != 1)
                        settings::SetSpeedX(settings::GetSpeedX()+p->m_value);
                    break;
                case 2:
                    if(settings::GetGamemode() != 1)
                        settings::SetSpeedX(settings::GetSpeedX()-p->m_value);
                    break;
                case 3:
                    settings::SetDownforce(settings::GetDownforce()+p->m_value);
                    break;
                case 4:
                    settings::SetDownforce(settings::GetDownforce()-p->m_value);
                default:
                    break;
                }
            }
        }
    }
}

bool Level::GetFinished()
{
    return m_finished;
}


void Level::Draw(int viewCenterX)
{
    int startTile = static_cast<int>((viewCenterX - 400) / m_tileSize.x) * m_tilesPerCollum;
    unsigned int endTile = startTile + ( (m_tilesPerRow+1) * m_tilesPerCollum);

    if(endTile > m_tiles.size())
        endTile = m_tiles.size();


    for(unsigned int i = startTile; i < endTile; i++)
    {
        m_window.draw(m_tiles[i]);
    }

}


bool Level::LoadTextures()
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
        if(line == "[Music]")
        {
            file.eof();
            std::getline(file, line);
            std::stringstream sstri;
            sstri << line;
            int temp;
            sstri >> temp;
            settings::SetMusic(temp);
        }

    }

    return true;
}

void Level::LoadTileData(int layer)
{
    std::stringstream sstr;
    sstr << m_path << "tileData" << layer << ".cfg";

    std::fstream file;
    file.open(sstr.str().c_str(), std::ios::in);

    std::string line;

    m_filledBlocks = 0;

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
                            sprite.setTextureRect(sf::IntRect(aw::conv::ToInt(tempXCoord),
                                                              aw::conv::ToInt(tempYCoord),
                                                              m_tileSize.x,
                                                              m_tileSize.y));

                            sprite.setRotation(aw::conv::ToInt(tempRotation));
                            sprite.setOrigin(0,0);

                            sprite.setTexture(m_textures[aw::conv::ToInt(tempNumTex)], false);

                            if(aw::conv::ToInt(tempRotation) == 90)
                            {
                                sprite.setOrigin(sprite.getPosition().x , sprite.getPosition().y + sprite.getLocalBounds().height);
                            }
                            if(aw::conv::ToInt(tempRotation) == 180)
                            {
                                sprite.setOrigin(sprite.getPosition().x + sprite.getLocalBounds().width, sprite.getPosition().y + sprite.getLocalBounds().height);
                            }
                            if(aw::conv::ToInt(tempRotation) == 270)
                            {
                                sprite.setOrigin(sprite.getPosition().x + sprite.getLocalBounds().width , sprite.getPosition().y);
                            }

                            m_tiles.push_back(sprite);
                            m_filledBlocks++;

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


void Level::LoadCollideGrid()
{
    std::string filePath = m_path + "collidegrid.cfg";

    std::fstream file;
    file.open(filePath.c_str(), std::ios::in);

    std::string line;

    m_collisionBlocks = 0;

    while(!file.eof())
    {
        std::getline(file, line);

        if(line == "[Data]")
        {
           while(!file.eof())
           {
                std::getline(file, line);
                std::string substr;
                for(unsigned int i = 0; i < line.size(); i++)
                {
                   substr.clear();
                   substr = line[i];
                   m_collision.push_back(aw::conv::ToInt(substr));
                   if(substr != "0")
                        m_collisionBlocks++;
                }
            }
        }
    }
}

void Level::LoadScripts()
{
    std::string filePath = m_path + "scripts.ini";

    std::fstream file;
    file.open(filePath.c_str(), std::ios::in);

    if(file.fail())
    {
        file.close();
        return;
    }

    std::string line;

    while(!file.eof())
    {
        std::getline(file, line);
        if(line == "[Script]")
        {
            m_scripts.push_back(Script());

            for(unsigned int i = 0; i < 3; i++)
            {
                file.eof();
                std::getline(file, line);

                if(i == 0)
                {
                    m_scripts[m_scripts.size()-1].m_type = aw::conv::ToInt(line);
                }
                if(i == 1)
                {
                    m_scripts[m_scripts.size()-1].m_value = aw::conv::ToInt(line);
                }
                if(i == 2)
                {
                    m_scripts[m_scripts.size()-1].m_posX = aw::conv::ToInt(line)* m_tileSize.x;
                }
            }
        }
    }
}

int Level::GetLevelLength()
{
    return m_levelLength;
}

sf::Vector2f Level::GetTileSize()
{
    return m_tileSize;
}

unsigned int Level::GetFilledBlocks()
{
    return m_filledBlocks;
}
unsigned int Level::GetCollisionBlocks()
{
    return m_collisionBlocks;
}
