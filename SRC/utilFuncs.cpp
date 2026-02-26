#include "utilFuncs.h"

#include <fstream>
#include <iostream>


int FindNumberOfCardsInSet(std::string setToOpen)
{
    std::ifstream setBeingRead("sets/"+setToOpen);
    if (!setBeingRead) {
        std::cerr << "Error opening file\n";
    }
    int lines = 0;
    std::string line;
    while (std::getline(setBeingRead, line))
    {
        lines++;
    }

    setBeingRead.close();
    return lines;
}