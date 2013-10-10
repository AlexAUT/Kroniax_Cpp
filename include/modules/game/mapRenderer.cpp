#include "mapRenderer.hpp"

#include <fstream>
#include <sstream>


#include <SFML/Graphics/RenderWindow.hpp>

const int TILESIZE = 25;

namespace aw
{
	bool MapRenderer::load(const std::string &path)
	{
		//Check if the level is already loaded
		if (path == m_path)
			return true;
		//Otherwise load the new level and unload the current one
		m_path = path;
		unload();

		std::fstream file(path.c_str(), std::ios::in);

		if (!file.good())
			return false;

		//Color vector
		std::vector<sf::Color> fillColors;

		std::string line;
		while (std::getline(file, line))
		{
			if (line == "[Colors]")
			{
				while (true)
				{
					std::getline(file, line);

					if (line == "[/Colors]")
						break;

					if (file.eof())
						return false;

					std::stringstream sstr(line);
					int r, g, b;
					sstr >> r >> g >> b;
					fillColors.push_back(sf::Color(r, g, b));
				} 
			}
			else if (line == "[Blocks]")
			{
				while (true)
				{
					std::getline(file, line);

					if (line == "[/Blocks]")
						break;

					if (file.eof())
						return false;

					//Add a new Collum
					m_collums.push_back(MapCollum());

					std::stringstream sstr(line);
					//Check the y start and endpos (100000 = no tiles in this collum)
					int startPos, endPos;
					sstr >> startPos >> endPos;

					if (startPos >= 100000 || endPos >= 100000)
						continue;

					for (int i = startPos; i <= endPos; i++)
					{
						std::size_t colorIndex;
						int colType; //Coltype not used here... its for the logic part
						sstr >> colorIndex >> colType;

						sf::Color fillColor;
						//Search for the fillColor in the fillColors-vector
						//Error checking. if not in the vector = black
						if (colorIndex < 0 || colorIndex >= fillColors.size())
						{
							fillColor = sf::Color::Black;
						}
						else
						{
							fillColor = fillColors[colorIndex];
						}

						sf::Vector2f position = sf::Vector2f(static_cast<float>((m_collums.size() - 1) * TILESIZE), static_cast<float>(i * TILESIZE));
						sf::Vector2f size = sf::Vector2f(static_cast<float>(TILESIZE), static_cast<float>(TILESIZE));
						//add the tile
						m_collums.back().addRect(position, size, fillColor);
					}
				}
			}
		}

		return true;
	}

	void MapRenderer::unload()
	{
		m_collums.clear();
		m_path = "";
	}

	void MapRenderer::render(sf::RenderWindow &window)
	{
		int startCol = static_cast<int>((window.getView().getCenter().x - (window.getView().getSize().x / 2.f)) / 25.f);


		int colToDraw = startCol + static_cast<int>(window.getView().getSize().x / 25.f) + 1;

		if (startCol < 0)
		{
			startCol = 0;
		}
		
		for (auto i = startCol; i < colToDraw; i++)
		{
			if (i < static_cast<int>(m_collums.size()))
				m_collums[i].render(window);
		}
	}
}