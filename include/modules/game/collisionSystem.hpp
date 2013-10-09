#ifndef AWCOLLISIONSYSTEM_HPP
#define AWCOLLISIONSYSTEM_HPP

#include "collisionCollum.hpp"

namespace aw
{
	//////FORWARD DECL /////
	class Player;

	class CollisionSystem
	{
	public:

		void loadMap(const std::string &path);

		CollisionType checkCollision(const Player &player);

	private:

	private:
		std::vector<CollisionCollum> m_collisionCollums;
	};
}

#endif //AWCOLLISIONSYSTEM_HPP