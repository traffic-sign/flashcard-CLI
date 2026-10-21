#include "utilFuncs.h"
#include "terminal.h"

#include <fstream>
#include <iostream>
#include <algorithm>


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
    return lines/2; //divide the number of lines by 2 since each card is 2 lines
}


std::string getUserInput(terminalRenderer& terminal, int x, int y, std::string question, bool lower, bool accountForSpaces)
{
    terminal.toggleCursor();
    terminal.writeText(question, x, y);
    terminal.render();
    terminal.moveCursor(1, question.length());
    
    std::string answer;
    std::cin >> answer;

    if (lower) std::transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
    if (accountForSpaces) {std::string answer_extra; std::getline(std::cin, answer_extra); answer = answer+answer_extra;}

    terminal.toggleCursor();
    return answer;
}
