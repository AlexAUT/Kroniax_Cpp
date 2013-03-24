#ifndef HIGHSCOREUPLOADER_HPP
#define HIGHSCOREUPLOADER_HPP

#include <vector>
#include <string>

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
    std::string GetError();

    std::vector<Score> &GetScore();

private:

    std::vector<Score> m_score;
    std::string m_errorMessage;

};

#endif // HIGHSCOREUPLOADER_HPP
