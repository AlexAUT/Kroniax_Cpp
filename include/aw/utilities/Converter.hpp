#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#include <string>
#include <vector>

namespace aw
{
    namespace conv
    {

        std::string ToString(int value);
        std::string ToString(unsigned int value);
        std::string ToString(float value);
        std::string ToString(double value);
        std::string ToString(bool value);


        int ToInt(std::string value);
        int ToInt(float value);

        float ToFloat(std::string value);
        float ToFloat(int value);

        std::vector<int> SplitStringIntoInt(std::string line);

    }
}

#endif // CONVERTER_HPP
