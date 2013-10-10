#include "collisionSystem.hpp"

#include <fstream>
#include <string>
#include <sstream>

#include "player.hpp"

namespace aw
{
	CollisionType CollisionSystem::checkCollision(const Player &player)
	{
		//Check every vertex for collision
		for (std::size_t i = 0; i < 3; ++i)
		{
			if (player.getVertexPosition(i).y < 0)
				continue;

			std::size_t xPosInTiles = static_cast<std::size_t>(player.getVertexPosition(i).x / 25.f);
			std::size_t yPosInTiles = static_cast<std::size_t>(player.getVertexPosition(i).y / 25.f);

			if (xPosInTiles < m_collisionCollums.size()-1)
			{
				auto result = m_collisionCollums[xPosInTiles].getCollision(yPosInTiles);

				if (result != CollisionType::NOTHING)
				{
					return result;
				}
			}
		}

		return CollisionType::NOTHING;
	}

	void CollisionSystem::loadMap(const std::string &path)
	{
		std::fstream file(path.c_str(), std::ios::in);

		if (!file.good())
			return;

		std::string line;
		while (std::getline(file, line))
		{
			if (line == "[Blocks]")
			{
				while (std::getline(file, line))
				{
					if (line == "[/Blocks]")
					{
						break;
					}

					std::stringstream sstr(line);
					std::size_t start, end;
					sstr >> start >> end;
					
					m_collisionCollums.push_back(CollisionCollum());

					if (start < 100000)
					{
						m_collisionCollums.back().setStartAndEnd(start, end);

						for (std::size_t i = start; i <= end; ++i)
						{
							int type;
							int useless;
							sstr >> useless >> type;
							if (type == 0)
							{
								m_collisionCollums.back().push_back(CollisionType::NOTHING);
							}
							else if (type == 1)
							{
								m_collisionCollums.back().push_back(CollisionType::WALL);
							}
							else if (type == 2)
							{
								m_collisionCollums.back().push_back(CollisionType::FINISH);
							}
						}
					}
					
				}
			}
		}

		file.close();
	}
}