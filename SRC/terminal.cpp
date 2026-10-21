#include "terminal.h"

#include <iostream>
#include <string>


void terminalRenderer::clear()
{
    buffer = "";

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            buffer = buffer+" ";
        }        
        buffer = buffer+'\n';
    }
}

void terminalRenderer::clearline(int line)
{
    for (int i = line*width+line; i < (line*width+line)+width; i++)
    {
        buffer[i] = ' '; 
    }
}
    
void terminalRenderer::writeText(std::string text, int x, int y)
{
    if (x > width || x < 0) throw std::invalid_argument("X value excedes the width or goes into the negatives!");
    if (y > height || y < 0) throw std::invalid_argument("Y value excedes the height or goes into the negatives!");

    int writingIndex = (width*y)+x+y; //the extra +y is for the '\n's that don't get counted in the height 
    int addedXvalue = -1;

    for (char charToWrite : text)
    {
        if (charToWrite == '\n') {writingIndex+=(width+x-addedXvalue); addedXvalue = -1; continue;}
        buffer[writingIndex] = charToWrite; 
        writingIndex++;
        addedXvalue++;
    }
}
    
void terminalRenderer::fill(char fillChar)
{
    buffer = "";

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            buffer = buffer+fillChar;
        }        
        buffer = buffer+'\n';
    }
}

void terminalRenderer::render()
{
    std::cout << "\033[2J\033[3J\033[H" << buffer;
}

terminalRenderer::terminalRenderer(int widthCON, int heightCON)
{
    width = widthCON;
    height = heightCON;
    buffer = "";

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            buffer = buffer+" ";
        }        
        buffer = buffer+'\n';
    }
}


void terminalRenderer::toggleCursor()
{
    if (cursorVisible)
    {
        std::cout << "\033[?25l";
        cursorVisible = false;
    }
    else
    {
        std::cout << "\033[?25h";
        cursorVisible = true;
    }
}

void terminalRenderer::moveCursor(int up, int leftRight)
{
    std::cout << "\033["+std::to_string(up)+"A\033["+std::to_string(leftRight)+"C"; //I don't even wanna explain this disgusting ass line
}

void terminalRenderer::haltUntilInput()
{
    moveCursor(1, 0);
    std::cin.get(); 
    clear();
}
