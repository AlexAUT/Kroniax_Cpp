#ifndef AWTIMETABLE_HPP
#define AWTIMETABLE_HPP

#include <string>
#include <vector>

#include <SFML/Graphics/Font.hpp>

namespace sf
{
	class RenderWindow;
}

namespace aw
{
	namespace priv
	{
		struct Player
		{
			std::string name;
			float time;

			Player() : Player("", 0)
			{}

			Player(const std::string &pName, float pTime):
				name(pName), time(pTime)
			{

			}
		};
	}

	class TimeTable
	{
	public:
		TimeTable();

		void addPlayer(const std::string &name, float time = 0);
		void removePlayer(const std::string &name);
		void removePlayer(std::size_t index);

		void addTime(const std::string &name, float time);
		void addTime(std::size_t index, float time);

		void resetTimes();

		void render(sf::RenderWindow &window);

		void clear();

		std::size_t getPlayerIndex(const std::string &name);
		float getTime(std::size_t index);

	private:

		void orderTimeTable();

	private:
		std::vector<priv::Player> m_players;

		sf::Font m_font;

	};
}
#endif