#ifndef AWCOLLISIONCOLLUM_HPP
#define AWCOLLISIONCOLLUM_HPP

#include <vector>

namespace aw
{
	enum class CollisionType
	{
		NOTHING,
		WALL,
		FINISH
	};

	class CollisionCollum
	{
	public:
		CollisionCollum();

		void setStartAndEnd(std::size_t start, std::size_t end);
		
		void push_back(CollisionType type);

		CollisionType getCollision(std::size_t yPosInTiles);

	private:

	private:
		std::size_t m_start, m_end;
		std::vector<CollisionType> m_blocks;

	};
}

#endif