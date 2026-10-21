#include "TerminalManager.h"
#include "terminal.h"
#include "utilFuncs.h"
#include "decks.h"

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <random>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;


void deleteLine(std::string fileName, int Delete) //I had to have been possessed by satan himself to write this horrid code I'm not even going to touch this for a while
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
    deck set;
    terminalRenderer terminal(100, 20);
    terminal.toggleCursor();

    while (true)
    {
        terminal.writeText("commands:\n\"help\"\n\"showAllSets\"\n\"newSet\"\n\"deleteSet\"\n\"openSet\"\n|\"addCards\"\n|\"removeCards\"\n|\"cards\"\n|\"showAllCards\"\n|\"write\"\n|\"closeSet\"\n\"kill\"\n", 0, 0);
        terminal.clearline(terminal.height-1);
        std::string input = getUserInput(terminal, 0, terminal.height-1, "input a command: ", true, false); //the -1 on the terminal.height is because the line indicies start at 0 and not 1

        if (input == "newset")
        {
            std::string name = getUserInput(terminal, 0, terminal.height-1, "type the name of your new set: ", true, false);

            if (!fs::exists("sets/"+name+".txt")) //if theres no errors it means it found the file and it exists which means it doesn't need to create a file
            {
                fs::path current_dir = fs::current_path();
                fs::path new_sets_path = current_dir / "sets";
                fs::create_directory(new_sets_path);
                std::ofstream newSet("sets/"+name+".txt");
                newSet.close();
            }
            else
            {
                terminal.writeText("Error! A set with that name already exists! Press any key to continue: ", 0, terminal.height-1);
                terminal.render();
                terminal.haltUntilInput();
            }
        }
        else if (input == "deleteset")
        {
            std::string name = getUserInput(terminal, 0, terminal.height-1, "type the name of the set you want to delete: ", true, false);

            if (fs::exists("sets/"+name+".txt")) //if theres no errors it means it found the file and it exists which means it doesn't need to create a file
            {
                std::remove(("sets/"+name+".txt").c_str());
            }
            else
            {
                terminal.writeText("Error! No set with that name exists! Press any key to continue: ", 0, terminal.height-1);
                terminal.render();
                terminal.haltUntilInput();
            }

        }
        else if (input == "showallsets")
        {
            terminal.clear();
            if (fs::is_directory("sets"))
            {
                int numSets = 0;

                terminal.writeText("------------------------------------", 0, 1);
                for (const auto& entry : fs::directory_iterator("sets")) {
                    numSets++;
                    std::string name = entry.path().filename().string(); //gets the set name
                    int numCards = FindNumberOfCardsInSet(name); //gets cards from the set BEFORE erasing the .txt on the name
                    name.erase(name.length()-4, 4); //erases the .txt
                    terminal.writeText(name+" - "+std::to_string(numCards)+" cards", 0, 1+numSets);
                }
                terminal.writeText("------------------------------------", 0, 1+numSets+1);

                terminal.writeText("Press any key to continue: ", 0, terminal.height-1);
                terminal.render();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                terminal.haltUntilInput();
            }
            else
            {
                terminal.writeText("Error! No sets directory! Press any key to continue: ", 0, terminal.height-1);
                terminal.render();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                terminal.haltUntilInput();
            }
        }
        else if (input == "openset")
        {
            std::string name = getUserInput(terminal, 0, terminal.height-1, "type the name of the set you want to open: ", true, false);
            
            set.openSet(name, "sets/"+name+".txt");
        }
        else if (input == "kill")
        {
            break;
        }
        else if (set.setOpened && input == "closeset")
        {
            set.killSet();
        }
        else if (set.setOpened && input == "cards")
        {            
            std::string shuffled = getUserInput(terminal, 0, terminal.height-1, "should the cards be shuffled? (y/n): ", true, false);
            if (shuffled == "y" || shuffled == "Y")
                set.shuffle();


            bool front = true;
            for (int i = 0; i < set.cardCount;)
            {
                terminal.clearline(terminal.height-2);
                terminal.writeText(((front == true) ? set.cards[i].first : set.cards[i].second), 0, terminal.height-2);
                terminal.clearline(terminal.height-1);
                std::string input = getUserInput(terminal, 0, terminal.height-1, "flip (f), next (n), or back (b): ", true, false);

                if (input == "f") //flip
                {
                    if (front) 
                        {front = false;}
                    else 
                        {front = true;}
                }        
                else if (input == "n") //next
                {
                    front = true;
                    i++;
                }
                else if (input == "b" && i > 0) //back. the !i == 0  in the if statement just makes sure that the user isn't trying to go back from the first card in the set.
                {
                    front = true;
                    i--;
                }
            }
            if (set.shuffled)
                set.restore();
            terminal.clear();
        }
        else if (set.setOpened && input == "write") //It works well enough to where I don't want to touch it anymore
        {
            std::string shuffled = getUserInput(terminal, 0, terminal.height-1, "should the cards be shuffled and have a mix of answering with definitions and terms? (y/n): ", true, false);
            if (shuffled == "y")
                set.flipAndShuffle();

            for (int i = 0; i < set.cardCount; i++)
            {
                terminal.clear();
                terminal.writeText(set.cards[i].first, 0, terminal.height-2);

                std::string answer = getUserInput(terminal, 0, terminal.height-1, "answer: ", true, true);

                if (answer == set.cards[i].second)
                {
                    terminal.clearline(terminal.height-1);
                    terminal.writeText("correct! enter anything to continue: ", 0, terminal.height-1);
                    terminal.render();  
                    terminal.haltUntilInput();
                }
                else
                {
                    terminal.clearline(terminal.height-1);
                    terminal.writeText("incorrect! the correct answer was: "+set.cards[i].first+" enter anything to continue ", 0, terminal.height-1);
                    terminal.render();  
                    
                    terminal.haltUntilInput();
                    set.cards.erase(set.cards.begin()+i);
                    set.cardCount--;
                    i--;
                }
            }
            set.restore(); //restores cards back to before write 
        }
        else if (set.setOpened && input == "addcards") //so many stds
        {
            while (true)
            {
                set.setStream.clear();
                set.setStream.seekg(0, std::ios::end); //brings the get pointer to the start of the set's fstream
                terminal.clearline(terminal.height-1);
                std::string term = getUserInput(terminal, 0, terminal.height-1, "Please enter a term: ", false, true);
                terminal.clearline(terminal.height-1);
                std::string definition = getUserInput(terminal, 0, terminal.height-1, "now enter the definition of that term: ", false, true);
                set.setStream << term << std::endl << definition << std::endl;

                std::pair<std::string, std::string> card(term, definition);
                set.cards.push_back(card);
                set.cardCount++;
                
                terminal.clearline(terminal.height-1);
                std::string answer = getUserInput(terminal, 0, terminal.height-1, "add another term? (y/n): ", true, false);

                if (answer == "y") {continue;}
                else {break;}
            }
        }
        else if (set.setOpened && input == "removecards")
        {
            std::string term;

            while (true)
            {
                set.setStream.clear();
                set.setStream.seekg(0, std::ios::end);
                terminal.clearline(terminal.height-1);
                std::string Delete = getUserInput(terminal, 0, terminal.height-1, "please enter the term or definition of the card you would like to delete: ", false, true);

                int index = 0;
                for (std::pair<std::string, std::string> card : set.cards)
                {
                    if (Delete == card.first)
                    {
                        set.setStream.clear();
                        set.setStream.seekg(0, std::ios::beg);
                        int i = 0;
                        while (true)
                        {
                            std::string line;
                            std::getline(set.setStream, line);

                            if (line == Delete)
                            {
                                set.setStream.close(); //closing the path so that deleteLine works
                                deleteLine(set.name, i+1);
                                deleteLine(set.name, i);
                                set.setStream.open(set.path); //reopening after deletion
                                std::erase(set.cards, card);
                                set.cardCount--;
                                break;
                            }
                            i++;
                        }
                    }
                    if (Delete == card.second)
                    {
                        set.setStream.clear();
                        set.setStream.seekg(0, std::ios::beg);
                        int i = 0;
                        while (true)
                        {
                            std::string line;
                            std::getline(set.setStream, line);

                            if (line == Delete)
                            {
                                set.setStream.close(); //closing the path so that deleteLine works
                                deleteLine(set.name, i-1);
                                deleteLine(set.name, i-1);
                                set.setStream.open(set.path); //reopening after deletion
                                std::erase(set.cards, card);
                                set.cardCount--;
                                break;
                            }
                            i++;
                        }
                    }
                    
                    index++;
                }

                terminal.clearline(terminal.height-1);
                std::string answer = getUserInput(terminal, 0, terminal.height-1, "delete another term? (y/n): ", false, true);

                if (answer == "y") {continue;}
                else {break;}
            }
        }
        else if (set.setOpened && input == "showallcards")
        {
            terminal.clear();
            terminal.writeText("------------------------------------", 0, 1);

            int numCards;
            for (numCards = 0; numCards < set.cardCount; numCards++)
            {
                terminal.writeText(set.cards[numCards].first+"  :  "+set.cards[numCards].second, 0, numCards+2);
            }

            terminal.writeText("------------------------------------", 0, numCards+3);
            terminal.writeText("Press any key to continue: ", 0, terminal.height-1);
            terminal.render();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            terminal.haltUntilInput();
        }
        else 
        {
            terminal.clearline(terminal.height-1);
            terminal.writeText("Error: no command by that name can be executed, press enter to continue", 0, terminal.height-1);
            terminal.render();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            terminal.haltUntilInput();
        }
    }
}
