#include "TerminalManager.h"
#include "utilFuncs.h"

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <random>
#include <vector>
#include <filesystem>


#define CUR_UP1 "\033[F"
#define DEL_LINE "\033[K"

namespace fs = std::filesystem;


void deleteLine(std::string fileName, int Delete) //this is so ugly but I really can't find any better way
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
    std::vector<std::pair<std::string, std::string>> cards;
            
    std::cout << "type \"help\" for a list of valid commands\n";
    while (true)
    {
        std::string input = getUserInput("\ninput a command: ", true, false);
        
        if (input == "help")
        {
            std::cout << "commands:\n \"help\"\n \"showAllSets\"\n \"newSet\" \n \"deleteSet\" \n \"openSet\" \n | \"addCards\"\n | \"removeCards\"\n | \"cards\"\n | \"showAllCards\"\n | \"write\"\n | \"closeSet\"\n \"kill\"\n";
        }
        else if (input == "newset")
        {
            std::string name = getUserInput("type the name of  your new set: ", true, false);

            std::ifstream setExists("sets/"+name+".txt");
            if (fs::exists("sets/"+name+".txt")) //if theres no errors it means it found the file and it exists which means it doesn't need to create a file
            {
                std::cout << "Error: a set with that name already exists!";
                setExists.close();
            }
            else
            {
                fs::path current_dir = fs::current_path();
                fs::path new_sets_path = current_dir / "sets";
                fs::create_directory(new_sets_path);
                std::ofstream newSet("sets/"+name+".txt");
                newSet.close();
            }

        }
        else if (input == "deleteset")
        {
            std::string name = getUserInput("type the name of the set you want to delete: ", true, false);

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
            std::cout << "\n\n----------------------------------------------------------------";
            if (fs::is_directory("sets"))
            {
                for (const auto& entry : fs::directory_iterator("sets")) {
                    std::string name = entry.path().filename().string(); //gets the set name
                    int numCards = FindNumberOfCardsInSet(name); //gets cards from the set BEFORE erasing the .txt on the name
                    name.erase(name.length()-4, 4); //erases the .txt
                    std::cout << "\n\n" << name << " - " << numCards << " cards";
                }
            }
            std::cout << "\n\n----------------------------------------------------------------\n\n";
        }
        else if (input == "openset")
        {
            std::string name = getUserInput("type the name of the set you want to open: ", true, false);
            
            std::ifstream setThere("sets/"+name+".txt");
            if (!setThere.good()) //makes sure it was able to open the file
            {
                std::cout << "Error: that set does not exist\n";
                continue;
            }
            setThere.close();
            set.open("sets/" + name + ".txt");
            setOpened = true;
            setPath = "sets/" + name + ".txt";
            setName = name;

            std::string term;
            while (std::getline(set, term))
            {
                std::string definition;
                if (!std::getline(set, definition)) break;
                std::pair<std::string, std::string> card = {term, definition};
                cards.push_back(card);
            }
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
            cards.clear();
        }
        else if (setOpened && input == "cards")
        {
            std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            std::vector<std::pair<std::string, std::string>> unshuffled_cards = cards;          
            
            std::string answer = getUserInput("should the cards be shuffled? (y/n): ", true, false);

            if (answer == "y" || answer == "Y")
            {
                std::random_device rd; //I genuinley have no clue what these lines do I just copied them. 
                std::mt19937 g(rd());  //all they do is shuffle the cards
        
                std::shuffle(cards.begin(), cards.end(), g);
            }

            bool front = true;
            for (int i = 0; i < cards.size();)
            {

                std::cout <<  CUR_UP1 DEL_LINE CUR_UP1 DEL_LINE; // move the cursor to go back up and write over the last card
                std::cout << ((front == true) ? cards[i].first : cards[i].second) << "\nflip (f), next (n), or back (b): ";
                std::string input = getUserInput("", true, false);

                if (input == "f") //flip
                {
                    if (front) 
                        {front = false;}
                    else 
                        {front = true;}
                }        
                if (input == "n") //next
                {
                    front = true;
                    i++;
                }
                if (input == "b" && !i == 0) //back. the !i == 0  in the if statement just makes sure that the user isn't trying to go back from the first card in the set.
                {
                    front = true;
                    i--;
                }
            }
            cards = unshuffled_cards;
        }
        else if (setOpened && input == "write") //everything under this if statement HAS to be satanic
        {
            std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            std::vector<std::pair<std::string, std::string>> preWriteCards = cards;

            std::random_device rd; //I genuinley have no clue what this lines do I just copied them. all they do shuffle the cards
            std::mt19937 g(rd());  // 
            std::uniform_int_distribution<int> distrib(0, 1); 
        
            std::shuffle(cards.begin(), cards.end(), g);
            for (int i = 0; i < cards.size(); i++)
            {
                bool flipped = distrib(g);
                std::cout << CUR_UP1 DEL_LINE CUR_UP1 DEL_LINE << ((flipped) ? cards[i].second : cards[i].first) << "\n: ";

                std::string answer = getUserInput("", true, true);

                if (answer == ((flipped) ? cards[i].first : cards[i].second))
                {
                    std::cout << CUR_UP1 DEL_LINE CUR_UP1 DEL_LINE "correct!\n: enter anything to continue ";
                    std::cin.get(); //holds the program until enter is pressed again
                }
                else
                {
                    std::cout << CUR_UP1 DEL_LINE CUR_UP1 DEL_LINE "incorrect! the correct answer was "+((flipped) ? cards[i].first : cards[i].second)+"\n: enter anything to continue (type N if you were correct) ";
                    std::string Continue;
                    std::getline(std::cin, Continue);
                    if (Continue != "N" && Continue != "n")
                    {
                        std::pair<std::string, std::string> savedCard = cards[i];
                        cards.erase(cards.begin()+i);
                        cards.push_back(savedCard);
                        i--;
                    }
                }
            }
            cards = preWriteCards; //restores cards back to before write 

        }
        else if (setOpened && input == "addcards") //so many stds
        {
            while (true)
            {
                set.clear();
                set.seekg(0, std::ios::end); //brings the get pointer to the start of the set's fstream
                std::string term = getUserInput("Please enter a term: ", false, true);
                std::string definition = getUserInput("now enter the definition of that term: \n", false, true);
                set << term << std::endl << definition << std::endl;

                std::string answer = getUserInput("add another term? (y/n): ", true, false);
                std::pair<std::string, std::string> card(term, definition);
                cards.push_back(card);

                if (answer == "y") {std::cout << CUR_UP1 DEL_LINE; continue;}
                else {break;}
            }
        }
        else if (setOpened && input == "removecards")
        {
            std::string term;

            while (true)
            {
                set.clear();
                set.seekg(0, std::ios::end);
                std::string Delete = getUserInput("please enter the term or definition of the card you would like to delete: ", false, true);

                int index = 0;
                for (std::pair<std::string, std::string> card : cards)
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
                                std::erase(cards, card);
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
                                std::erase(cards, card);
                                break;
                            }
                            i++;
                        }
                    }
                    
                    index++;
                }

                std::string answer = getUserInput("delete another term? (y/n): ", false, true);

                if (answer == "y") {std::cout << CUR_UP1 DEL_LINE; continue;}
                else {break;}
            }
        }
        else if (setOpened && input == "showallcards")
        {
            std::cout << "\n\n";
            std::cout << "------------------------------------";
            for (int i = 0; i < cards.size(); i++)
            {
                std::cout << "\n\n" << cards[i].first << "  :  " << cards[i].second;
            }
            std::cout << "\n\n------------------------------------";
        }
        else 
        {
            std::cout << "Error: no command by that name exists type in \"help\" for a list of valid commands\n";
        }
    }
}
