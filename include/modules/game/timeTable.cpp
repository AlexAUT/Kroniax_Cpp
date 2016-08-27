#include "timeTable.hpp"

#include "../../global.hpp"

#include <sstream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>


namespace aw
{
	TimeTable::TimeTable()
	{
		m_font.loadFromFile(Helper::getData("fonts/visitor1.ttf"));
	}

	void TimeTable::addPlayer(const std::string &name, float time, const sf::Color &color)
	{
		m_currentHighscore.push_back(priv::Player(name, time, color));
		//Set up the array
	}

	void TimeTable::removePlayer(const std::string &name)
	{
		for (auto it = m_currentHighscore.begin(); it != m_currentHighscore.end(); ++it)
		{
			if (it->name == name)
			{
				m_currentHighscore.erase(it);
				break;
			}
		}
	}
	
	void TimeTable::removePlayer(std::size_t index)
	{
		if (index < m_currentHighscore.size())
		{
			m_currentHighscore.erase(m_currentHighscore.begin() + index);
		}
	}

	void TimeTable::addTime(const std::string &name, float time)
	{
		//Serach the player
		std::size_t index = getPlayerIndex(name);
		//Add the new time
		m_currentHighscore[index].time = time;
	}

	void TimeTable::addTime(std::size_t index, float time)
	{
		if (index < m_currentHighscore.size())
		{
			m_currentHighscore[index].time = time;
		}
	}

	void TimeTable::addLadderTime(const std::string &name, float time)
	{
		m_ladder.push_back(priv::Player(name, time, sf::Color::White));
	}

	void TimeTable::clearLadder()
	{
		m_ladder.clear();
	}

	void TimeTable::resetTimes()
	{
		for (auto &it : m_currentHighscore)
		{
			it.time = 0;
		}
	}

	void TimeTable::render(sf::RenderWindow &window)
	{
		sf::RectangleShape overlay(sf::Vector2f(800, 450));
		overlay.setFillColor(sf::Color(0, 0, 0, 180));
		//The highscore of this current game
		sf::Text toDraw;
		toDraw.setFont(m_font);

		toDraw.setPosition(175, 50);
		toDraw.setString("Timetable");
		toDraw.setCharacterSize(35);
		window.draw(toDraw);

		toDraw.setCharacterSize(18);

		for (std::size_t i = 0; i < 20; ++i)
		{
			if (i >= m_currentHighscore.size())
			{
				break;
			}
			else
			{
				toDraw.setPosition(sf::Vector2f((static_cast<int>(i / 10) * 250) + 35.f, 120 + (i % 10) * 25.f));
				std::stringstream sstr;
				if (m_currentHighscore[i].time > 0.5f)
				{
					sstr << (i+1) << ": " << m_currentHighscore[i].time << " sec  ";
				}
				else
				{
					sstr << (i+1) <<  " no time  ";
				}
				toDraw.setString(sstr.str() + m_currentHighscore[i].name);
				toDraw.setColor(m_currentHighscore[i].color);
				window.draw(toDraw);
			}
		}

		//LADDER of the current MAP
		toDraw.setPosition(600, 50);
		toDraw.setString("Ladder");
		toDraw.setCharacterSize(35);
		toDraw.setColor(sf::Color::White);
		window.draw(toDraw);
		toDraw.setCharacterSize(18);
		for (std::size_t i = 0; i < m_ladder.size(); ++i)
		{
			toDraw.setPosition(sf::Vector2f(550.f, 120 + i * 25.f));
			std::stringstream sstr;
			sstr << (i + 1) << ": " << m_ladder[i].time << " sec  " << m_ladder[i].name;
			toDraw.setString(sstr.str());
			window.draw(toDraw);
		}
	}

	void TimeTable::clear()
	{
		m_currentHighscore.clear();
	}


	std::size_t TimeTable::getPlayerIndex(const std::string &name)
	{
		for (std::size_t i = 0; i < m_currentHighscore.size(); ++i)
		{
			if (name == m_currentHighscore[i].name)
			{
				return i;
			}
		}

		return 0;
	}

	float TimeTable::getTime(std::size_t index)
	{
		if (index < m_currentHighscore.size())
		{
			if (m_currentHighscore[index].time < 1)
			{
				return 1000000;
			}
			else
			{
				return m_currentHighscore[index].time;
			}
		}

		return 1000000;
	}

	void TimeTable::orderTimeTable()
	{
		
	}
}