#ifndef DIALOGSCREEN_HPP
#define DIALOGSCREEN_HPP

namespace sf
{
	class RenderWindow;
}

#include <string>

namespace db
{

    void DialogOK(sf::RenderWindow &window, std::string text);
    bool DialogYesNo(sf::RenderWindow &window, std::string text);

    std::string InputDialog(sf::RenderWindow &window, std::string text, std::string inputText);

}


#endif // DialogScreen_HPP
