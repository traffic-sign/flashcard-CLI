#include "TerminalManager.h"
#include "utilFuncs.h"

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <random>
#include <vector>
#include <filesystem>


#define CUR_UP1 "\033[F"
#define CUR_DOWN1 "\033[K"

namespace fs = std::filesystem;


void deleteLine(std::string fileName, int Delete)
{
    std::ifstream inFile("sets/"+fileName+".txt");
    std::ofstream outFile("sets/temp.txt");

    std::string line;
    int currentLine = 0;

    while (std::getline(inFile, line)) {
        if (currentLine != Delete) {
            outFile << line << "\n";
        }
        currentLine++;
    }

    inFile.close();
    outFile.close();

    std::cout << fileName << '\n';
    const char* path = ("sets/"+fileName+".txt").c_str();
    
    std::remove(path);
    std::rename("sets/temp.txt", ("sets/"+fileName+".txt").c_str());
}

void startTerminalLoop()
{
    bool setOpened = false;
    std::fstream set;
    std::string setPath = "";
    std::string setName = "";
    std::cout << "type \"help\" for a list of valid commands)\n";
    while (true)
    {
        std::string input;
        std::cout << "\ninput a command: ";
        std::cin >> input;
        std::transform(input.begin(), input.end(), input.begin(), ::tolower); //makes the input all lowercase. the first input.begin() and input.end() specify the
        //of where the transformation happens, the second input.begin() just needs to be there I forgot why, and ::tolower is the operation thats being performed on it
        
        if (input == "help")
        {
            std::cout << "commands:\n \"help\"\n \"showAllSets\"\n \"newSet\" \n \"deleteSet\" \n \"openSet\" \n | \"addCards\"\n | \"removeCards\"\n | \"cards\"\n | \"showAllCards\"\n | \"write\"\n | \"closeSet\"\n \"kill\"\n";
        }
        else if (input == "newset")
        {
            std::cout << "type the name of your new set: ";
            std::string name;
            std::cin >> name;

            std::ifstream setExists("sets/"+name+".txt");
            if (setExists.good()) //if theres no errors it means it found the file and it exists which means it doesn't need to create a file
            {
                std::cout << "Error: a set with that name already exists!";
                setExists.close();
            }
            else
            {
                std::ofstream newSet("sets/"+name+".txt");
                newSet.close();
            }

        }
        else if (input == "deleteset")
        {
            std::cout << "type the name of the setyou want to delete: ";
            std::string name;
            std::cin >> name;

            std::ifstream setExists("sets/"+name+".txt");
            if (setExists.good()) //if theres no errors it means it found the file and it exists which means it doesn't need to create a file
            {
                std::cout << "Succsesfully deleted set\n";
                setExists.close();
                std::remove(("sets/"+name+".txt").c_str());
            }
            else
            {
                std::cout << "Error: no set with that name already exists!";
                setExists.close();
            }

        }
        else if (input == "showallsets")
        {
            std::cout << "\n\n ----------------------------------------------------------------";
            for (const auto& entry : fs::directory_iterator("sets")) {
                std::string name = entry.path().filename().string(); //gets the set name
                int numCards = FindNumberOfCardsInSet(name); //gets cards from the set BEFORE erasing the .txt on the name
                name.erase(name.length()-4, 4); //erases the .txt
                std::cout << "\n\n" << name << " - " << numCards << " cards";
            }
            std::cout << "\n\n----------------------------------------------------------------\n\n";
        }
        else if (input == "openset")
        {
            std::cout << "type the name of the set you would like to open: ";
            std::string Name;
            std::cin >> Name;
            
            std::ifstream setThere("sets/"+Name+".txt");
            if (!setThere.good()) //makes sure it was able to open the file
            {
                std::cout << "Error: that set does not exist\n";
                continue;
            }
            setThere.close();
            set.open("sets/" + Name + ".txt");
            setOpened = true;
            setPath = "sets/" + Name + ".txt";
            setName = Name;
        }
        else if (input == "kill")
        {
            break;
        }
        else if (setOpened && input == "closeset")
        {
            set.close();
            setOpened = false;
            setPath = "";
            setName = "";
        }
        else if (setOpened && input == "cards")
        {
            std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            std::vector<std::pair<std::string, std::string>> cards;
            
            std::string term;

            int index = 0;
            set.clear();
            set.seekg(0, std::ios::beg);
            while (std::getline(set, term))
            {
                std::string definition;
                if (!std::getline(set, definition)) break;
                std::pair<std::string, std::string> card = {term, definition};
                cards.push_back(card);

                index++;
            }        
            std::random_device rd; //I genuinley have no clue what this lines do I just copied them. all they do shuffle the cards
            std::mt19937 g(rd());  // 
        
            std::shuffle(cards.begin(), cards.end(), g);
            for (int i = 0; i < cards.size(); i++)
            {
                bool front = true;

                std::cout << cards[i].first << "\nflip (f), next (n), or back (b): ";
                flip:
                std::string input;
                std::cin >> input;

                if (input == "f")
                {
                    if (front) 
                        {std::cout << CUR_UP1 CUR_UP1 CUR_UP1 CUR_DOWN1 << cards[i].second << "\nflip (f), next (n), or back (b): "; front = false;}
                    else    
                        {std::cout << CUR_UP1 CUR_UP1 CUR_UP1 CUR_DOWN1 << cards[i].first << "\nflip (f), next (n), or back (b): "; front = true;}
                    goto flip;
                }             
                if (input == "n")
                {
                    std::cout << CUR_UP1 CUR_DOWN1 CUR_UP1 CUR_DOWN1;
                    continue;
                }
                if (input == "b")
                {
                    std::cout <<  CUR_UP1 CUR_DOWN1  CUR_UP1 CUR_DOWN1;
                    if (!i == 0)
                        i-=2;
                    else
                        i--;
                }
            }
        }
        else if (setOpened && input == "write")
        {
            std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            std::vector<std::pair<std::string, std::string>> cards;
            
            std::string term;

            int index = 0;
            set.clear();
            set.seekg(0, std::ios::beg);
            while (std::getline(set, term))
            {
                std::string definition;
                if (!std::getline(set, definition)) break;
                std::pair<std::string, std::string> card;
                if (rand()%2 == 1)
                    card = {term, definition};
                else
                    card = {definition, term};
                 
                cards.push_back(card);

                index++;
            }        

            std::random_device rd; //I genuinley have no clue what this lines do I just copied them. all they do shuffle the cards
            std::mt19937 g(rd());  // 
        
            std::shuffle(cards.begin(), cards.end(), g);
            for (int i = 0; i < cards.size(); i++)
            {
                preAnswer:
                std::cout << cards[i].first << "\n: ";

                std::string answer;
                std::cin >> answer;
                std::string answer_extra;
                std::getline(std::cin, answer_extra);
                answer = answer+answer_extra;

                if (answer == cards[i].second)
                {
                    std::cout << CUR_UP1 CUR_DOWN1 CUR_UP1 CUR_DOWN1 << "correct!\n: enter anything to continue ";
                    std::string Continue;
                    std::getline(std::cin, Continue);
                    std::cout << CUR_UP1 CUR_DOWN1 CUR_UP1 CUR_DOWN1;
                }
                else
                {
                    std::cout << CUR_UP1 CUR_DOWN1 CUR_UP1 CUR_DOWN1 << "incorrect! the correct answer was " << cards[i].second << "\n: enter anything to continue (type N if you were correct) ";
                    
                    std::string Continue;
                    std::getline(std::cin, Continue);
                    std::cout << CUR_UP1 CUR_DOWN1 CUR_UP1 CUR_DOWN1;

                    if (Continue != "N")
                    {
                        std::pair<std::string, std::string> savedCard = cards[i];
                        cards.erase(cards.begin()+i);
                        cards.push_back(savedCard);
                        i--;
                    }
                }
            }

        }
        else if (setOpened && input == "addcards")
        {
            while (true)
            {
                set.clear();
                set.seekg(0, std::ios::end);
                std::cout << "please enter the term: " << std::endl;
                std::string term; 
                std::cin >> term;
                std::string extraTerms;
                std::getline(std::cin, extraTerms);
                term = term+extraTerms;
                std::cout << CUR_UP1 CUR_DOWN1 CUR_UP1 CUR_DOWN1 << "now enter the definition of that term: " << std::endl;
                std::string definition; 
                std::cin >> definition;
                std::string extraDefs;
                std::getline(std::cin, extraDefs);
                definition = definition+extraDefs;
                set << term << std::endl << definition << std::endl;

                std::cout << CUR_UP1 CUR_DOWN1 CUR_UP1 CUR_DOWN1 << "add another term? (y/n): "; 
                std::string answer;
                std::cin >> answer;

                if (answer == "y") {std::cout << CUR_UP1 CUR_DOWN1; continue;}
                else {break;}
            }
        }
        else if (setOpened && input == "removecards")
        {
            std::vector<std::pair<std::string, std::string>> cards;
            
            std::string term;

            int index = 0;
            set.clear();
            set.seekg(0, std::ios::beg);
            while (std::getline(set, term))
            {
                std::string definition;
                if (!std::getline(set, definition)) break;
                std::pair<std::string, std::string> card = {term, definition};
                cards.push_back(card);

                index++;
            }
            while (true)
            {
                set.clear();
                set.seekg(0, std::ios::end);
                std::cout << "please enter the term or definition of the card you would like to delete: " << std::endl;
                std::string Delete; 
                std::cin >> Delete;
                std::string extraDeletes;
                std::getline(std::cin, extraDeletes);
                Delete = Delete+extraDeletes;

                int index = 0;
                for (std::pair<std::string, std::string> card : cards)
                {
                    if (Delete == card.first || Delete == card.second)
                    {
                        if (Delete == card.first)
                        {
                            set.clear();
                            set.seekg(0, std::ios::beg);
                            int i = 0;
                            while (true)
                            {
                                std::string line;
                                std::getline(set, line);

                                if (line == Delete)
                                {
                                    set.close(); //closing the path so that deleteLine works
                                    deleteLine(setName, i+1);
                                    deleteLine(setName, i);
                                    set.open(setPath); //reopening after deletion
                                    break;
                                }
                                i++;
                            }
                        }
                        if (Delete == card.second)
                        {
                            set.clear();
                            set.seekg(0, std::ios::beg);
                            int i = 0;
                            while (true)
                            {
                                std::string line;
                                std::getline(set, line);

                                if (line == Delete)
                                {
                                    set.close(); //closing the path so that deleteLine works
                                    deleteLine(setName, i-1);
                                    deleteLine(setName, i-1);
                                    set.open(setPath); //reopening after deletion
                                    break;
                                }
                                i++;
                            }
                        }
                        break;
                    }
                    index++;
                }

                std::cout << CUR_UP1 CUR_DOWN1 << "delete another term? (y/n): "; 
                std::string answer;
                std::cin >> answer;

                if (answer == "y") {std::cout << CUR_UP1 CUR_DOWN1; continue;}
                else {break;}
            }
        }
        else if (setOpened && input == "showallcards")
        {
            std::cout << "\n\n";
            std::vector<std::pair<std::string, std::string>> cards;
            
            std::string term;

            int index = 0;
            set.clear();
            set.seekg(0, std::ios::beg);
            while (std::getline(set, term))
            {
                std::string definition;
                if (!std::getline(set, definition)) break;
                std::pair<std::string, std::string> card = {term, definition};
                cards.push_back(card);

                index++;
            }        
            std::cout << "------------------------------------";
            for (int i = 0; i < cards.size(); i++)
            {
                std::cout << "\n\n" << cards[i].second << "  :  " << cards[i].first;
            }
            std::cout << "\n\n------------------------------------";
        }
        else 
        {
            std::cout << "Error: no command by that name exists type in \"help\" for a list of valid commands\n";
        }
    }
}
