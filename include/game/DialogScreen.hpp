#ifndef DIALOGSCREEN_HPP
#define DIALOGSCREEN_HPP

#include "../../include/gui/GuiController.hpp"

namespace db
{

    void DialogOK(sf::RenderWindow &window, std::string text);
    bool DialogYesNo(sf::RenderWindow &window, std::string text);

    std::string InputDialog(sf::RenderWindow &window, std::string text, std::string inputText);

}


#endif // DialogScreen_HPP
