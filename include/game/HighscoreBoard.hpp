#ifndef HIGHSCOREBOARD_HPP
#define HIGHSCOREBOARD_HPP

#include "../../include/game/HighscoreUploader.hpp"

namespace sf
{
	class RenderWindow;
}

#include <vector>
#include <string>

void ListHighscore(sf::RenderWindow &window, std::string lvlname, std::vector<Score> &m_scores);

#endif // HIGHSCOREBOARD_HPP
