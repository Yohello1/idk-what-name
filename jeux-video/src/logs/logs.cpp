#include "logs.hpp"

namespace chlorine::logging
{
    // Terminal

    logToTerminal::logToTerminal(std::string prefix, std::string colorCode)
    {
        _prefix = prefix;
        _colorCode = colorCode;
        std::cout << _colorCode << "Creating class to output to terminal with prefix "  << _prefix << " and colors currently being used\n";
    }

    void logToTerminal::log(std::string value)
    {
        // Look at the second answer by Medh
        // https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c
        std::cout << _colorCode << _prefix << value << "\033[0m\t\t";
    }


    // To file
    logToFile::logToFile(std::string file, std::string prefix)
    {
        _file = file;
        _prefix = prefix;
        _filestream.open(file);
        std::cout << "Outputting to file " << _file << " with internal prefix: " << _prefix << "\n";
    }

    void logToFile::log(std::string value)
    {
        _filestream << value;
    }

    logToFile::~logToFile()
    {
        _filestream.close();
    }

}
