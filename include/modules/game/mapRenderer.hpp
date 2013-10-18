#ifndef AWMAPRENDERER_HPP
#define AWMAPRENDERER_HPP

//////INCLUDES/////
#include <string>
#include <vector>

#include "mapCollum.hpp"


namespace aw
{
	/////CLASS DECL./////
	class MapRenderer
	{
	public:
		bool load(const std::string &path);
		void unload();

		void render(sf::RenderWindow &window);

		bool isOnTheEnd(std::size_t xPos);

	private:

	private:
		std::string m_path;

		std::vector<MapCollum> m_collums;
		
	};
}

#endif //AWMAPRENDERER_HPP