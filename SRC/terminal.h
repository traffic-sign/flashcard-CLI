#ifndef _TERMINAL_
#define _TERMINAL_

#include <iostream>
#include <string>

struct terminalRenderer
{
    int width;
    int height;
    std::string buffer;

    bool cursorVisible = true;

    terminalRenderer(int widthCON, int heightCON);

    void render();

    void clear();
    void clearline(int line);
    void haltUntilInput();
    void fill(char fillChar);
    void writeText(std::string text, int x, int y);
    void toggleCursor();
    void moveCursor(int up, int leftRight);
};


#endif