#include "decks.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>

void deck::openSet(std::string NAME, std::string PATH)
{
    setStream.open(PATH, std::ios::in | std::ios::out);

    if (setStream.is_open()) 
    {
        setOpened = true;
        name = NAME;
        path = PATH;
        setStream.seekg(0);
        setStream.clear();

        std::string definition;
        std::string term;
        std::pair<std::string, std::string> card;

        int card_count = 0;
        while (std::getline(setStream, definition))
        {
            std::getline(setStream, term);
            card.first = definition; card.second = term;
            untransformableCards.push_back(card);
            cards.push_back(card);
            card_count++;
        }

        cardCount = card_count;
    }
    else
        std::cout << "Error: that set does not exist\n";
}

void deck::killSet()
{
    setOpened = false;

    name = "";
    path = "";
    cardCount = -1;
    setStream.close();
    cards.clear();
}


void deck::shuffle()
{
    std::random_device rd; //I genuinley have no clue what these lines do I just copied them. 
    std::mt19937 g(rd());  //all they do is shuffle the cards
        
    std::shuffle(cards.begin(), cards.end(), g);
    shuffled = true;
}

void deck::restore()
{
    cards = untransformableCards;
    shuffled = false;
}

void deck::flipAndShuffle()
{
    std::random_device rd; //I genuinley have no clue what these lines do I just copied them. 
    std::mt19937 g(rd());  //all they do is shuffle the cards
    std::uniform_int_distribution<int> distrib(0, 1); 
        
    std::shuffle(cards.begin(), cards.end(), g);

    for (int i = 0; i < cardCount; i++)
    {
        if (distrib(g) == 1)
        {
            std::string temp = cards[i].first;
            cards[i].first = cards[i].second;
            cards[i].second = temp;
        }
    }

    shuffled = true;
}


