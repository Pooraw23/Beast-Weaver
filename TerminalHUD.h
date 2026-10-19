#ifndef TERMINALHUD_H
#define TERMINALHUD_H

#include <iostream>
#include <string>

using namespace std;

class TerminalHUD {
public:
    // ANSI Color Escape Constants (Slide 03 - Const qualification)
    static const string RESET;
    static const string BOLD;
    static const string RED;
    static const string GREEN;
    static const string YELLOW;
    static const string BLUE;
    static const string MAGENTA;
    static const string CYAN;
    static const string WHITE;

    // Static rendering utilities (Slide 04 & 05)
    static string colorize(const string& text, const string& color);
    static string renderColorHpBar(int current, int max, int length = 16);
    static string getElementalBadge(int type);
    static string getStatusBadge(int status);
    static void printHeaderBanner(const string& title);
    static void flashMessage(const string& msg, const string& color);
};

#endif
