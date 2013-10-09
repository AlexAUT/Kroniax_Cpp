#include "collisionCollum.hpp"

namespace aw
{
	CollisionCollum::CollisionCollum() :
		m_start(100000), m_end(100000)
	{

	}

	void CollisionCollum::setStartAndEnd(std::size_t start, std::size_t end)
	{
		m_start = start;
		m_end = end;
	}

	void CollisionCollum::push_back(CollisionType type)
	{
		m_blocks.push_back(type);
	}

	CollisionType CollisionCollum::getCollision(std::size_t yPosInTiles)
	{
		if (yPosInTiles >= m_start && yPosInTiles <= m_end)
		{
			return m_blocks[yPosInTiles - m_start];
		}
		else
		{
			return CollisionType::NOTHING;
		}
	}
}





