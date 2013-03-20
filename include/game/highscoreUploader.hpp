#ifndef HIGHSCOREUPLOADER_HPP
#define HIGHSCOREUPLOADER_HPP

#include <SFML/Network.hpp>
#include <vector>
#include <iostream>

struct Score
{
    std::string name;
    unsigned int score;
};

class HighscoreUploader
{
public:

    HighscoreUploader();

    bool Submit(std::string lvlname, std::string name, unsigned int length, unsigned int filledBlocks, unsigned int collisionBlocks, unsigned int score); // Upload the highscore

    bool GetHighscore(std::string levelname);

    std::vector<Score> &GetScore();

private:

private:

    std::vector<Score> m_score;

};

#endif // HIGHSCOREUPLOADER_HPP
