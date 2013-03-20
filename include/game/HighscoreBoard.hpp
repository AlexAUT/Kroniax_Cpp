#ifndef HIGHSCOREBOARD
#define HIGHSCOREBOARD

#include "HighscoreUploader.hpp"
#include "../../include/aw/utilities/converter.hpp"

#include <SFML/Graphics.hpp>

void ListHighscore(sf::RenderWindow &window, std::string lvlname, std::vector<Score> &m_scores);

#endif // HIGHSCOREBOARD
