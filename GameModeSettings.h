#ifndef GAMEMODESETTINGS_H
#define GAMEMODESETTINGS_H

#include <iostream>
#include <string>

using namespace std;

class GameModeSettings {
private:
    static bool nuzlockeEnabled;   // Permadeath for fainted beasts (Slide 05)
    static bool speedrunEnabled;
    static int speedrunMaxTurns;

public:
    static void setNuzlocke(bool enable);
    static void setSpeedrun(bool enable, int maxTurns = 120);

    static bool isNuzlocke();
    static bool isSpeedrun();
    static int getTurnsRemaining();
    static bool checkSpeedrunFail();
    static string getStatusSummary();
};

#endif
