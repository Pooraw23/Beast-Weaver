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
    static int timePeriod;    // 0 = Morning, 1 = Afternoon, 2 = Dusk, 3 = Night
    static int weatherState;  // 0 = Clear, 1 = Scorching Sun, 2 = Torrential Rain, 3 = Mystic Fog

public:
    // Static functions to manipulate world time and weather
    static void advanceTime(int turns = 1);
    static void restUntilMorning();
    static string getTimeOfDayString();
    static string getTimeIcon();
    static string getWeatherString();
    static string getWeatherIcon();
    static int getDay();
    static int getTimePeriod();
    static int getWeather();
    static bool isNight();
    static int getTotalTurns();
    static void setWorldTime(int day, int period, int turns, int weather = 0);

    // Stream operator overloading for UI header display and persistence (Slide 06 & 07)
    friend ostream& operator<<(ostream& os, const WorldState& ws);
    friend ofstream& operator<<(ofstream& out, const WorldState& ws);
    friend ifstream& operator>>(ifstream& in, WorldState& ws);
};

#endif
