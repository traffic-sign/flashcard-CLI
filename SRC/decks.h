#ifndef _DECKS_H_
#define _DECKS_H_

#include <vector>
#include <string>
#include <fstream>

struct deck
{
    std::string name;
    std::string path;
    std::fstream setStream;
    int cardCount;
    std::vector<std::pair<std::string, std::string>> untransformableCards;
    std::vector<std::pair<std::string, std::string>> cards;
    bool setOpened = false;
    bool shuffled;

    deck() {};
    void openSet(std::string NAME, std::string PATH);
    void killSet();
    void shuffle();
    void flipAndShuffle();
    void restore();
    void flushCardVectorToFile();
};

#endif
