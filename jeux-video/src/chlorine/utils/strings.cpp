#include <chlorine/utils/strings.hpp>

namespace chlorine::utils
{
    std::size_t splitStringToVector(const std::string &txt, std::vector<std::string> &strs, char ch)
    {
        size_t pos = txt.find( ch );
        size_t initialPos = 0;
        strs.clear();

        // Decompose statement
        while( pos != std::string::npos ) {
            strs.push_back( txt.substr( initialPos, pos - initialPos ) );
            initialPos = pos + 1;

            pos = txt.find( ch, initialPos );
        }

        // Add the last one
        strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

        return strs.size();
    }

    std::size_t stripString(std::string &str, std::string &subStr)
    {
        std::size_t startPos = str.find(subStr);
        str.erase(startPos, str.length() - startPos);
        return str.length();
    }
}
