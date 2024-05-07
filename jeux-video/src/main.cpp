#include <iostream>

#include "logs/logs.hpp"

int main()
{
    std::cout << "Hello world" << std::endl;

    chlorine::logging::logToTerminal helloWorld{"LOG: ", "\x1B[32m"};
    helloWorld.log("hi\n");

    // chlorine::logging::logToFile goodBye("log.txt", "textLog: ");
    // goodBye.log("baii");
}
