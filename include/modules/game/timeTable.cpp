#include "timeTable.hpp"

#include <sstream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>


namespace aw
{
	TimeTable::TimeTable()
	{
		m_font.loadFromFile("data/fonts/visitor1.ttf");
	}

	void TimeTable::addPlayer(const std::string &name, float time)
	{
		m_players.push_back(priv::Player(name, time));
		//Set up the array
	}

	void TimeTable::removePlayer(const std::string &name)
	{
		for (auto it = m_players.begin(); it != m_players.end(); ++it)
		{
			if (it->name == name)
			{
				m_players.erase(it);
				break;
			}
		}
	}
	
	void TimeTable::removePlayer(std::size_t index)
	{
		if (index < m_players.size())
		{
			m_players.erase(m_players.begin() + index);
		}
	}

	void TimeTable::addTime(const std::string &name, float time)
	{
		//Serach the player
		std::size_t index = getPlayerIndex(name);
		//Add the new time
		m_players[index].time = time;
	}

	void TimeTable::addTime(std::size_t index, float time)
	{
		if (index < m_players.size())
		{
			m_players[index].time = time;
		}
	}

	void TimeTable::resetTimes()
	{
		for (auto &it : m_players)
		{
			it.time = 0;
		}
	}

	void TimeTable::render(sf::RenderWindow &window)
	{
		sf::RectangleShape overlay(sf::Vector2f(800, 450));
		overlay.setFillColor(sf::Color(0, 0, 0, 180));

		sf::Text toDraw;
		toDraw.setFont(m_font);

		toDraw.setPosition(250, 50);
		toDraw.setString("Scoreboard");
		toDraw.setCharacterSize(45);
		window.draw(toDraw);

		toDraw.setCharacterSize(25);

		for (std::size_t i = 0; i < 20; ++i)
		{
			if (i >= m_players.size())
			{
				toDraw.setPosition(sf::Vector2f((static_cast<int>(i / 10) * 350) + 100.f, 120 + i * 25.f - (static_cast<int>(i / 10) * 250)));
				toDraw.setString("Dummy");
				window.draw(toDraw);
				//break;
			}
			else
			{
				toDraw.setPosition(sf::Vector2f((static_cast<int>(i / 10) * 300) + 100.f, 120 + i * 25.f));
				std::stringstream sstr;
				if (m_players[i].time > 0.5f)
				{
					sstr << m_players[i].time << "sec";
				}
				else
				{
					sstr << "no time";
				}
				toDraw.setString(m_players[i].name + "   " + sstr.str());
				window.draw(toDraw);
			}
		}
	}

	void TimeTable::clear()
	{
		m_players.clear();
	}


	std::size_t TimeTable::getPlayerIndex(const std::string &name)
	{
		for (std::size_t i = 0; i < m_players.size(); ++i)
		{
			if (name == m_players[i].name)
			{
				return i;
			}
		}

		return 0;
	}

	float TimeTable::getTime(std::size_t index)
	{
		if (index < m_players.size())
		{
			if (m_players[index].time < 1)
			{
				return 1000000;
			}
			else
			{
				return m_players[index].time;
			}
		}

		return 1000000;
	}

	void TimeTable::orderTimeTable()
	{
		
	}
}