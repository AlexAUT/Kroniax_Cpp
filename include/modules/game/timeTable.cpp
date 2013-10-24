#include "timeTabel.hpp"

#include <sstream>
#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>


namespace aw
{
	TimeTable::TimeTable()
	{
		m_font.loadFromFile("data/fonts/DejaVuSans.ttf");
	}

	void TimeTable::addPlayer(const std::string &name, float time)
	{
		std::cout << "Added Player: " << name << std::endl;
		m_players.push_back(priv::Player(name, time));
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
		for (auto it = m_players.begin(); it != m_players.end(); ++it)
		{
			if (it->name == name)
			{
				it->time = time;
			}
		}
	}

	void TimeTable::render(sf::RenderWindow &window)
	{
		sf::RectangleShape overlay(sf::Vector2f(800, 450));
		overlay.setFillColor(sf::Color(0, 0, 0, 180));

		sf::Text toDraw;
		toDraw.setFont(m_font);

		for (std::size_t i = 0; i < 10; ++i)
		{
			if (i >= m_players.size())
			{
				break;
			}
			else
			{
				toDraw.setPosition(sf::Vector2f(200.f, i * 25.f));
				std::stringstream sstr;
				sstr << m_players[i].time;
				toDraw.setString(m_players[i].name + sstr.str());
				window.draw(toDraw);
			}
		}
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
}