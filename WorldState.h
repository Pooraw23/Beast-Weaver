#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class WorldState {
private:
    // Static class variables tracking global world state independent of any single object (Slide 05)
    static int globalTurnCount;
    static int currentDay;
    static int timePeriod; // 0 = Morning (Dawn), 1 = Afternoon (Day), 2 = Dusk, 3 = Night

public:
    // Static functions to manipulate world time
    static void advanceTime(int turns = 1);
    static void restUntilMorning();
    static string getTimeOfDayString();
    static string getTimeIcon();
    static int getDay();
    static int getTimePeriod();
    static bool isNight();
    static int getTotalTurns();
    static void setWorldTime(int day, int period, int turns);

    // Stream operator overloading for UI header display and persistence (Slide 06 & 07)
    friend ostream& operator<<(ostream& os, const WorldState& ws);
    friend ofstream& operator<<(ofstream& out, const WorldState& ws);
    friend ifstream& operator>>(ifstream& in, WorldState& ws);
};

#endif
