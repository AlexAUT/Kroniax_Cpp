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
			sf::Color color;

			Player() : Player("", 0, sf::Color::White)
			{}

			Player(const std::string &pName, float pTime, const sf::Color &pColor):
				name(pName), time(pTime), color(pColor)
			{

			}
		};
	}

	class TimeTable
	{
	public:
		TimeTable();

		void addPlayer(const std::string &name, float time, const sf::Color &color);
		void removePlayer(const std::string &name);
		void removePlayer(std::size_t index);

		void addTime(const std::string &name, float time);
		void addTime(std::size_t index, float time);

		void addLadderTime(const std::string &name, float time);
		void clearLadder();

		void resetTimes();

		void render(sf::RenderWindow &window);

		void clear();

		std::size_t getPlayerIndex(const std::string &name);
		float getTime(std::size_t index);

	private:

		void orderTimeTable();

	private:
		std::vector<priv::Player> m_currentHighscore;
		std::vector<priv::Player> m_ladder;

		sf::Font m_font;

	};
}
#endif