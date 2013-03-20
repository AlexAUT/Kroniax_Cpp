#include"../../../include/aw/utilities/converter.hpp"
#include<iostream>
namespace aw
{

    namespace conv
    {

        std::string ToString(int value)
        {
            std::stringstream sstr;
            std::string conv;

            sstr << value;

            conv = sstr.str();

            return conv;
        }

        std::string ToString(unsigned int value)
        {
            std::stringstream sstr;
            std::string conv;

            sstr << value;

            conv = sstr.str();

            return conv;
        }

        std::string ToString(float value)
        {
            std::stringstream sstr;
            std::string conv;

            sstr << value;

            conv = sstr.str();

            return conv;
        }

        std::string ToString(double value)
        {
            std::stringstream sstr;
            std::string conv;

            sstr << value;

            conv = sstr.str();

            return conv;
        }

        std::string ToString(bool value)
        {
            std::string conv;

            if(value)
            {
                conv = "true";
            }
            else
            {
                conv = "false";
            }

            return conv;
        }


        int ToInt(std::string value)
        {
            std::stringstream sstr;
            int conv;

            sstr << value;
            sstr >> conv;

            return conv;
        }

        int ToInt(float value)
        {
            int temp = value;

            float temp2 = value - temp;

            if(temp2 >= 0.5f)
            {
                return ++temp;
            }else
            {
                return temp;
            }
        }

        float ToFloat(std::string value)
        {
            std::stringstream sstr;
            float conv;

            sstr << value;
            sstr >> conv;

            return conv;
        }
        float ToFloat(int value)
        {
            float conv = value;

            return conv;
        }


        std::vector<int> SplitStringIntoInt(std::string line)
        {
            std::vector<int> value;
            std::string substr;                     //To Save the pieces

            for(unsigned int i = 0; i < line.size(); i++)
            {
                if(line[i] != ',')
                {
                    substr = substr + line[i];
                }
                else
                {
                    value.push_back(conv::ToInt(substr));

                    substr.clear();
                }

                if(i == line.size()-1)
                {
                    value.push_back(conv::ToInt(substr));

                    substr.clear();
                }
            }

            return value;
        }

    }
}
