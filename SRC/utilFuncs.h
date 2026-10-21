#ifndef _UTILFUNCS_H_
#define _UTILFUNCS_H_

#include "terminal.h"

int FindNumberOfCardsInSet(std::string setToOpen);
std::string getUserInput(terminalRenderer& terminal, int x, int y, std::string question, bool lower, bool accountForSpaces);


#endif
